/**
 * @file drv8305_api.c
 * @brief DRV8305 Motor Gate Driver IC - Core Implementation
 * @details Implements the main state machine logic, SPI communication protocol,
 *          and register processing for the DRV8305 gate driver.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This implementation file contains:
 *   - Main state machine polling function (initialization, status, control cycles)
 *   - Status register reading state machine (4 status registers)
 *   - Control register programming state machine (7 control registers)
 *   - SPI packet creation and parsing functions
 *   - Register parameter parsing (configuration to SPI format)
 *   - Timer and delay management
 * 
 * @architecture
 * Three-tier hierarchical state machine:
 *   1. Main SM: Controls overall driver flow (init, idle, wake, sleep, status, control)
 *   2. Status SM: Sequentially reads 4 status registers (0x01-0x04)
 *   3. Control SM: Sequentially writes 7 control registers (0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C)
 * 
 * @spi_protocol
 * Write Packet: [R/W=0 (1 bit)] [Register Address (4 bits)] [Data (11 bits)]
 * Read Packet:  [R/W=1 (1 bit)] [Register Address (4 bits)] [Reserved (11 bits)]
 * Response:     [Fault (1 bit)] [Register Address (4 bits)] [Data (11 bits)]
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "drv8305_macros.h"
#include "drv8305_register_map.h"
#include "DRV8305_Config/drv8305_configuration.h"
#include "DRV8305_Control_Registers/drv8305_control_registers_definitions.h"
#include "drv8305_api.h"

#include "DRV8305_Status_Registers/drv8305_status_registers_handlers.h"
#include "DRV8305_Control_Registers/drv8305_control_registers_handlers.h"

/* -------------------------------- FUNCTION PROTOTYPES -------------------------------- */
DRV8305_PRIVATE void     drv8305_status_register_process_polling  (drv8305_user_object_t *self);
DRV8305_PRIVATE void     drv8305_control_register_process_polling (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_spi_write_command_process        (drv8305_user_object_t *self, drv8305_register_types_t drv8305_register, uint16_t data);
DRV8305_PRIVATE uint16_t drv8305_spi_read_command_process         (drv8305_user_object_t *self, drv8305_register_types_t drv8305_register);
DRV8305_PRIVATE void     drv8305_main_sm_go_to_next_state         (drv8305_user_object_t *self, drv8305_sm_state_e next_state, uint32_t delay_time);
DRV8305_PRIVATE void     drv8305_status_sm_go_to_next_state       (drv8305_user_object_t *self, drv8305_status_sm_state_e next_state, uint32_t delay_time);
DRV8305_PRIVATE void     drv8305_control_sm_go_to_next_state      (drv8305_user_object_t *self, drv8305_control_sm_state_e next_state, uint32_t delay_time);
DRV8305_PRIVATE uint16_t drv8305_spi_write_packet_create          (drv8305_register_types_t register_type, uint16_t data);
DRV8305_PRIVATE uint16_t drv8305_spi_read_packet_create           (drv8305_register_types_t register_type);
DRV8305_PRIVATE uint16_t drv8305_spi_response_packet_create       (uint16_t data);
DRV8305_PRIVATE uint16_t drv8305_control_register_05_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_06_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_07_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_09_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_0A_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_0B_parser       (drv8305_user_object_t *self);
DRV8305_PRIVATE uint16_t drv8305_control_register_0C_parser       (drv8305_user_object_t *self);


/**
 * @brief Array of all DRV8305 register types (addresses) to be managed
 * @details Indexed array containing the register types in sequential order:
 *          Status registers (0x01-0x04) followed by control registers (0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C)
 * @see DRV8305_NUMBER_OF_REGISTERS
 */
DRV8305_PRIVATE const drv8305_register_types_t drv8305_registers[DRV8305_NUMBER_OF_REGISTERS] = 
{
     DRV8305_STATUS_01,
     DRV8305_STATUS_02,
     DRV8305_STATUS_03,
     DRV8305_STATUS_04,
     DRV8305_CONTROL_05,
     DRV8305_CONTROL_06,
     DRV8305_CONTROL_07,
     DRV8305_CONTROL_09,
     DRV8305_CONTROL_0A,
     DRV8305_CONTROL_0B,
     DRV8305_CONTROL_0C
};

extern drv8305_configuration_t default_configuration;

/* -------------------------------- PUBLIC FUNCTIONS -------------------------------- */

/**
 * @brief Initialize DRV8305 driver (implementation)
 * @details Initializes all driver structures, validates callbacks, and loads configuration.
 *          Calls wake_up and disable IO at startup.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Validates all required callbacks are non-NULL before initialization
 * @see drv8305_api_initialize (declaration in header)
 */
DRV8305_PUBLIC void drv8305_api_initialize(drv8305_user_object_t *self)
{
    if(self                                       == NULL ||
       self->hw_callbacks.drv8305_disable_io      == NULL ||
       self->hw_callbacks.drv8305_enable_io       == NULL || 
       self->hw_callbacks.drv8305_sleep_io        == NULL || 
       self->hw_callbacks.drv8305_wake_up_io      == NULL || 
       self->hw_callbacks.drv8305_spi_receive_cb  == NULL || 
       self->hw_callbacks.drv8305_spi_transmit_cb == NULL) { return; }

    self->state.cycle_time    = 0;
    self->state.delay_time    = 0;
    self->state.main_state    = DRV8305_INIT_STATE;
    self->state.status_state  = DRV8305_SM_STATUS_WARNING_REG;
    self->state.control_state = DRV8305_SM_CONTROL_HS_GATE_DRIVE_REG;

    self->hw_callbacks.drv8305_wake_up_io();
    self->hw_callbacks.drv8305_disable_io();

    drv8305_configuration_t* temp_config = drv8305_get_configuration();
    
    memset(&self->config, 0, sizeof(drv8305_configuration_t));
    memcpy(&self->config, temp_config, sizeof(drv8305_configuration_t));

    for(int index = 0; index < DRV8305_NUMBER_OF_REGISTERS; index++)
    {
        self->register_manager[index].data = 0;
        self->register_manager[index].type = drv8305_registers[index];        
    }
}

/**
 * @brief Execute main state machine iteration (implementation)
 * @details Runs one cycle of the main state machine handling initialization,
 *          idle, wake-up, sleep, status reading, and control register programming.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_master_sm_polling (declaration)
 */
DRV8305_PUBLIC void drv8305_api_master_sm_polling(drv8305_user_object_t *self)
{
    if(!self) { return; }

    switch (self->state.main_state)
    {
        case DRV8305_INIT_STATE:
        {
            self->hw_callbacks.drv8305_enable_io();
            self->hw_callbacks.drv8305_wake_up_io();
            drv8305_main_sm_go_to_next_state(self, DRV8305_CONTROL_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }

        case DRV8305_IDLE_STATE:
        {
            if(self->state.cycle_time >= DRV8305_STATUS_POLLING_INTERVAL_MS)
            {
                drv8305_main_sm_go_to_next_state(self, DRV8305_STATUS_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);
            }
            break;
        }

        case DRV8305_WAKE_UP_STATE:
        {
            self->hw_callbacks.drv8305_wake_up_io();
            drv8305_main_sm_go_to_next_state(self, DRV8305_IDLE_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }

        case DRV8305_SLEEP_STATE:
        {
            self->hw_callbacks.drv8305_sleep_io();
            drv8305_main_sm_go_to_next_state(self, DRV8305_IDLE_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }

        case DRV8305_STATUS_STATE:
        {
            drv8305_status_register_process_polling(self);

            break;
        }

        case DRV8305_CONTROL_STATE:
        {
            drv8305_control_register_process_polling(self);

            break;
        }

        case DRV8305_DELAY_STATE:
        {
            if(self->state.cycle_time >= self->state.delay_time)
            {
                self->state.main_state  = self->state.next_main_state;
            }

            break;
        } 
    }
}

/**
 * @brief Increment driver internal timer (implementation)
 * @details Increments cycle_time counter for state machine timing.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_timer (declaration)
 */
DRV8305_PUBLIC void drv8305_api_timer(drv8305_user_object_t *self)
{
    self->state.cycle_time++;
}

/**
 * @brief Enable DRV8305 IC (implementation)
 * @details Calls hardware enable callback to power up gate drivers.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_enable (declaration)
 */
DRV8305_PUBLIC void drv8305_api_ic_enable(drv8305_user_object_t *self)
{
    self->hw_callbacks.drv8305_enable_io();
}

/**
 * @brief Disable DRV8305 IC (implementation)
 * @details Calls hardware disable callback to power down gate drivers.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_disable (declaration)
 */
DRV8305_PUBLIC void drv8305_api_ic_disable(drv8305_user_object_t *self)
{
    self->hw_callbacks.drv8305_disable_io();
}

/**
 * @brief Get current DRV8305 configuration (pointer to default config)
 * @details Returns pointer to default_configuration structure containing all
 *          control register settings (gate drive, IC operation, sensing, etc.)
 * @return drv8305_configuration_t* Pointer to default configuration structure
 * @see drv8305_set_configuration, drv8305_configuration_t
 */
DRV8305_PUBLIC drv8305_configuration_t* drv8305_get_configuration(void)
{
    return &default_configuration;
}

/**
 * @brief Set new DRV8305 configuration
 * @details Copies provided configuration structure into default_configuration,
 *          replacing all current settings with new values.
 * @param[in] cfg Pointer to new configuration structure to copy
 * @return None
 * @note Configuration takes effect after drv8305_api_confirm_configuration() is called
 * @see drv8305_get_configuration, drv8305_configuration_t
 */
DRV8305_PUBLIC void drv8305_set_configuration(drv8305_configuration_t *cfg)
{
    memset(&default_configuration, 0, sizeof(drv8305_configuration_t));
    memcpy(&default_configuration, cfg, sizeof(drv8305_configuration_t));
}


/**
 * @brief Confirm configuration and start control register programming (implementation)
 * @details Transitions state machine to CONTROL_STATE to begin writing control registers.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_confirm_configuration (declaration)
 */
DRV8305_PUBLIC void drv8305_api_confirm_configuration(drv8305_user_object_t *self)
{
    drv8305_main_sm_go_to_next_state(self, DRV8305_CONTROL_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);
}

/* -------------------------------- PRIVATE FUNCTIONS -------------------------------- */

/**
 * @brief Process status register reading state machine (internal)
 * @details Sequentially reads all status registers (0x01 through 0x04) and triggers
 *          corresponding status callbacks for each register.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Internal state machine handling - called from drv8305_api_master_sm_polling()
 */
DRV8305_PRIVATE void drv8305_status_register_process_polling(drv8305_user_object_t *self)
{
    switch (self->state.status_state)
    {
        case DRV8305_SM_STATUS_WARNING_REG:
        {
            self->register_manager[DRV8305_STATUS_01_ARRAY_INDEX].data = drv8305_spi_read_command_process(self, self->register_manager[DRV8305_STATUS_01_ARRAY_INDEX].type);
            self->status_callbacks.drv8305_warning_register_cb(self, self->register_manager[DRV8305_STATUS_01_ARRAY_INDEX].data);
            drv8305_status_sm_go_to_next_state(self, DRV8305_SM_STATUS_OV_VDS_REG, DRV8305_STANDARD_TASK_DELAY_TIMEOUT);

            break;
        }

        case DRV8305_SM_STATUS_OV_VDS_REG:
        {
            self->register_manager[DRV8305_STATUS_02_ARRAY_INDEX].data = drv8305_spi_read_command_process(self, self->register_manager[DRV8305_STATUS_02_ARRAY_INDEX].type);
            self->status_callbacks.drv8305_ov_vds_register_cb(self, self->register_manager[DRV8305_STATUS_02_ARRAY_INDEX].data);
            drv8305_status_sm_go_to_next_state(self, DRV8305_SM_STATUS_IC_FAULTS_REG, DRV8305_STANDARD_TASK_DELAY_TIMEOUT);

            break;
        }

        case DRV8305_SM_STATUS_IC_FAULTS_REG:
        {
            self->register_manager[DRV8305_STATUS_03_ARRAY_INDEX].data = drv8305_spi_read_command_process(self, self->register_manager[DRV8305_STATUS_03_ARRAY_INDEX].type);
            self->status_callbacks.drv8305_ic_faults_register_cb(self, self->register_manager[DRV8305_STATUS_03_ARRAY_INDEX].data);
            drv8305_status_sm_go_to_next_state(self, DRV8305_SM_STATUS_VGS_FAULTS_REG, DRV8305_STANDARD_TASK_DELAY_TIMEOUT);

            break;
        }

        case DRV8305_SM_STATUS_VGS_FAULTS_REG:
        {
            self->register_manager[DRV8305_STATUS_04_ARRAY_INDEX].data = drv8305_spi_read_command_process(self, self->register_manager[DRV8305_STATUS_04_ARRAY_INDEX].type);
            self->status_callbacks.drv8305_vgs_faults_register_cb(self, self->register_manager[DRV8305_STATUS_04_ARRAY_INDEX].data);
            drv8305_main_sm_go_to_next_state(self, DRV8305_IDLE_STATE, DRV8305_STANDARD_TASK_DELAY_TIMEOUT);

            break;
        }

        case DRV8305_SM_STATUS_CYCLE_DELAY:
        {
            if(self->state.cycle_time >= self->state.delay_time)
            {
                self->state.status_state = self->state.next_status_state;
            }

            break;
        } 
    }
}

/**
 * @brief Process control register programming state machine (internal)
 * @details Sequentially writes all control registers (0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C)
 *          with configuration values and triggers corresponding control callbacks.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Internal state machine handling - called from drv8305_api_master_sm_polling()
 */
DRV8305_PRIVATE void drv8305_control_register_process_polling(drv8305_user_object_t *self)
{
    switch (self->state.control_state)
    {
        case DRV8305_SM_CONTROL_HS_GATE_DRIVE_REG:
        {
            self->register_manager[DRV8305_CONTROL_05_ARRAY_INDEX].data = drv8305_control_register_05_parser(self);
            self->register_manager[DRV8305_CONTROL_05_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_05_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_05_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_hs_gate_drive_control_register_cb(self, self->register_manager[DRV8305_CONTROL_05_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_LS_GATE_DRIVE_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }

        case DRV8305_SM_CONTROL_LS_GATE_DRIVE_REG:
        {
            self->register_manager[DRV8305_CONTROL_06_ARRAY_INDEX].data = drv8305_control_register_06_parser(self);
            self->register_manager[DRV8305_CONTROL_06_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_06_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_06_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_ls_gate_drive_control_register_cb(self, self->register_manager[DRV8305_CONTROL_06_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_GATE_DRIVE_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }
        
        case DRV8305_SM_CONTROL_GATE_DRIVE_REG:
        {
            self->register_manager[DRV8305_CONTROL_07_ARRAY_INDEX].data = drv8305_control_register_07_parser(self);
            self->register_manager[DRV8305_CONTROL_07_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_07_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_07_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_gate_drive_control_register_cb(self, self->register_manager[DRV8305_CONTROL_07_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_IC_OPERATION_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }
        
        case DRV8305_SM_CONTROL_IC_OPERATION_REG:
        {
            self->register_manager[DRV8305_CONTROL_09_ARRAY_INDEX].data = drv8305_control_register_09_parser(self);
            self->register_manager[DRV8305_CONTROL_09_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_09_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_09_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_ic_operation_register_cb(self, self->register_manager[DRV8305_CONTROL_09_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_SHUNT_AMPLIFIER_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }
        
        case DRV8305_SM_CONTROL_SHUNT_AMPLIFIER_REG:
        {
            self->register_manager[DRV8305_CONTROL_0A_ARRAY_INDEX].data = drv8305_control_register_0A_parser(self);
            self->register_manager[DRV8305_CONTROL_0A_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_0A_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_0A_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_shunt_amplifier_control_register_cb(self, self->register_manager[DRV8305_CONTROL_0A_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_VOLTAGE_REGULATOR_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }
        
        case DRV8305_SM_CONTROL_VOLTAGE_REGULATOR_REG:
        {
            self->register_manager[DRV8305_CONTROL_0B_ARRAY_INDEX].data = drv8305_control_register_0B_parser(self);
            self->register_manager[DRV8305_CONTROL_0B_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_0B_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_0B_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_voltage_regulator_control_register_cb(self, self->register_manager[DRV8305_CONTROL_0B_ARRAY_INDEX].data);
            drv8305_control_sm_go_to_next_state(self, DRV8305_SM_CONTROL_VDS_SENSE_REG, DRV8305_REGISTER_SWITCH_DELAY_MS);    

            break;
        }

        case DRV8305_SM_CONTROL_VDS_SENSE_REG:
        {
            self->register_manager[DRV8305_CONTROL_0C_ARRAY_INDEX].data = drv8305_control_register_0C_parser(self);
            self->register_manager[DRV8305_CONTROL_0C_ARRAY_INDEX].data = drv8305_spi_write_command_process(self, self->register_manager[DRV8305_CONTROL_0C_ARRAY_INDEX].type, self->register_manager[DRV8305_CONTROL_0C_ARRAY_INDEX].data);
            self->control_callbacks.drv8305_vds_sense_control_register_cb(self, self->register_manager[DRV8305_CONTROL_0C_ARRAY_INDEX].data);
            drv8305_main_sm_go_to_next_state(self, DRV8305_IDLE_STATE, DRV8305_REGISTER_SWITCH_DELAY_MS);

            break;
        }

        case DRV8305_SM_CONTROL_CYCLE_DELAY:
        {
            if(self->state.cycle_time >= self->state.delay_time)
            {
                self->state.control_state = self->state.next_control_state;
            }

            break;
        } 
    }
}

/**
 * @brief Execute SPI write command and receive response (internal)
 * @details Creates SPI write packet for register, transmits via callback, and returns
 *          parsed response data from hardware SPI receive callback.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] drv8305_register Register address to write
 * @param[in] data Data value to write to register
 * @return uint16_t SPI response data (lower 11 bits valid)
 * @note Uses hardware SPI callbacks configured during initialization
 */
DRV8305_PRIVATE uint16_t drv8305_spi_write_command_process(drv8305_user_object_t *self, drv8305_register_types_t drv8305_register, uint16_t data)
{
    uint16_t drv8305_tx_packet = drv8305_spi_write_packet_create(drv8305_register, data);
    self->hw_callbacks.drv8305_spi_transmit_cb(drv8305_tx_packet);

    uint16_t drv8305_rx_packet = drv8305_spi_response_packet_create(self->hw_callbacks.drv8305_spi_receive_cb());

    return drv8305_rx_packet;
}

/**
 * @brief Execute SPI read command and receive response (internal)
 * @details Creates SPI read packet for register, transmits via callback, and returns
 *          parsed response data from hardware SPI receive callback.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] drv8305_register Register address to read
 * @return uint16_t SPI response data containing register contents (lower 11 bits)
 * @note Uses hardware SPI callbacks configured during initialization
 */
DRV8305_PRIVATE uint16_t drv8305_spi_read_command_process(drv8305_user_object_t *self, drv8305_register_types_t drv8305_register)
{
    uint16_t drv8305_tx_packet = drv8305_spi_read_packet_create(drv8305_register);
    self->hw_callbacks.drv8305_spi_transmit_cb(drv8305_tx_packet);

    uint16_t drv8305_rx_packet = drv8305_spi_response_packet_create(self->hw_callbacks.drv8305_spi_receive_cb());

    return drv8305_rx_packet;
}

/**
 * @brief Schedule main state machine transition with delay (internal)
 * @details Prepares transition to next_state after specified delay_time cycles.
 *          Enters DRV8305_DELAY_STATE intermediate state.
 * @param[in,out] self Pointer to DRV8305 user object
 * @param[in] next_state Target state to transition to
 * @param[in] delay_time Number of cycles to wait before transition
 * @return None
 * @note Delay is synchronized with drv8305_api_timer() call rate
 */
DRV8305_PRIVATE void drv8305_main_sm_go_to_next_state(drv8305_user_object_t *self, drv8305_sm_state_e next_state, uint32_t delay_time)
{
    self->state.cycle_time      = 0;
    self->state.main_state      = DRV8305_DELAY_STATE;
    self->state.next_main_state = next_state;
    self->state.delay_time      = delay_time;
}

/**
 * @brief Schedule status state machine transition with delay (internal)
 * @details Prepares transition to next_state in status SM after delay_time cycles.
 *          Enters status DRV8305_STATUS_DELAY_STATE intermediate state.
 * @param[in,out] self Pointer to DRV8305 user object
 * @param[in] next_state Target status state to transition to
 * @param[in] delay_time Number of cycles to wait before transition
 * @return None
 */
DRV8305_PRIVATE void drv8305_status_sm_go_to_next_state(drv8305_user_object_t *self, drv8305_status_sm_state_e next_state, uint32_t delay_time)
{
    self->state.cycle_time        = 0;
    self->state.status_state      = DRV8305_SM_STATUS_CYCLE_DELAY;
    self->state.next_status_state = next_state;
    self->state.delay_time        = delay_time;
}

/**
 * @brief Schedule control state machine transition with delay (internal)
 * @details Prepares transition to next_state in control SM after delay_time cycles.
 *          Enters control DRV8305_SM_CONTROL_CYCLE_DELAY intermediate state.
 * @param[in,out] self Pointer to DRV8305 user object
 * @param[in] next_state Target control state to transition to
 * @param[in] delay_time Number of cycles to wait before transition
 * @return None
 */
DRV8305_PRIVATE void drv8305_control_sm_go_to_next_state(drv8305_user_object_t *self, drv8305_control_sm_state_e next_state, uint32_t delay_time)
{
    self->state.cycle_time         = 0;
    self->state.control_state      = DRV8305_SM_CONTROL_CYCLE_DELAY;
    self->state.next_control_state = next_state;
    self->state.delay_time         = delay_time;
}

/**
 * @brief Create SPI write packet from register and data (internal)
 * @details Formats register address and data into SPI command packet format.
 *          Packet structure: [R/W bit (0)] [Register Address (bits 14:11)] [Data (bits 10:0)]
 * @param[in] register_type Register address to write
 * @param[in] data Data value to write (lower 11 bits used)
 * @return uint16_t SPI write command packet
 */
DRV8305_PRIVATE uint16_t drv8305_spi_write_packet_create(drv8305_register_types_t register_type, uint16_t data)
{
    return ((register_type & 0x0F) << 11) | (data & 0x7FF);
}

/**
 * @brief Create SPI read packet from register address (internal)
 * @details Formats register address into SPI read command packet.
 *          Packet structure: [R/W bit (1)] [Register Address (bits 14:11)] [Reserved (bits 10:0)]
 * @param[in] register_type Register address to read
 * @return uint16_t SPI read command packet
 */
DRV8305_PRIVATE uint16_t drv8305_spi_read_packet_create(drv8305_register_types_t register_type)
{
    return (1 << 15) | ((register_type & 0x0F) << 11);
}

/**
 * @brief Parse and extract data from SPI response packet (internal)
 * @details Extracts the lower 11 bits containing register data from raw SPI response.
 * @param[in] data Raw SPI response packet
 * @return uint16_t Extracted register data (lower 11 bits)
 */
DRV8305_PRIVATE uint16_t drv8305_spi_response_packet_create(uint16_t data)
{
    return (data & 0x7FF);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_05_parser(drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL05(self->config.hs_gate_drive);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_06_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL06(self->config.ls_gate_drive);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_07_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL07(self->config.gate_drive);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_09_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL09(self->config.ic_operation);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_0A_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL0A(self->config.shunt_amplifier);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_0B_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL0B(self->config.voltage_regulator);
}

DRV8305_PRIVATE uint16_t drv8305_control_register_0C_parser (drv8305_user_object_t *self)
{
    return DRV8305_PACK_CTRL0C(self->config.vds_sense);
}
