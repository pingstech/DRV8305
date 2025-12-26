/**
 * @file drv8305_api.h
 * @brief DRV8305 Motor Gate Driver IC - Public API Interface
 * @details Declares public API functions and core data structures for the DRV8305 gate driver.
 *          Provides state machine management, register interface, and callback registration.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This header file defines the main public interface for the DRV8305 driver:
 *   - Register type enumeration and user object structure
 *   - State machine state definitions (main, status, control)
 *   - Callback structures for hardware abstraction
 *   - Public API functions for driver initialization and control
 * 
 * @dependencies
 *   - drv8305_register_map.h (register address definitions)
 *   - drv8305_configuration.h (configuration structures)
 */

#ifndef DRV8305_API_H_
#define DRV8305_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "drv8305_macros.h"
#include "drv8305_register_map.h"
#include "DRV8305_Config/drv8305_configuration.h"

/**
 * @brief DRV8305 Register Address Map
 * Defines the register addresses for status (read-only) and control (read/write) registers.
 */
typedef enum
{
    // Status registers (read-only)
    DRV8305_STATUS_01  = DRV8305_STATUS_01_REG_ADDR, // warning
    DRV8305_STATUS_02  = DRV8305_STATUS_02_REG_ADDR, // ov_vds_faults
    DRV8305_STATUS_03  = DRV8305_STATUS_03_REG_ADDR, // ic_faults
    DRV8305_STATUS_04  = DRV8305_STATUS_04_REG_ADDR, // vgs_faults

    // Control registers (read/write)
    DRV8305_CONTROL_05 = DRV8305_CONTROL_05_REG_ADDR, // HS Gate Drive Control
    DRV8305_CONTROL_06 = DRV8305_CONTROL_06_REG_ADDR, // LS Gate Drive Control
    DRV8305_CONTROL_07 = DRV8305_CONTROL_07_REG_ADDR, // Gate Drive Control
    DRV8305_CONTROL_09 = DRV8305_CONTROL_09_REG_ADDR, // IC Operation
    DRV8305_CONTROL_0A = DRV8305_CONTROL_0A_REG_ADDR, // Shunt Amplifier Control
    DRV8305_CONTROL_0B = DRV8305_CONTROL_0B_REG_ADDR, // Voltage Regulator Control
    DRV8305_CONTROL_0C = DRV8305_CONTROL_0C_REG_ADDR, // VDS Sense Control
} drv8305_register_types_t;

typedef enum
{
    DRV8305_INIT_STATE,    // -> Initialize all driver registers
    DRV8305_IDLE_STATE,    // -> Idle state
    DRV8305_STATUS_STATE,  // -> DRV8305 status process
    DRV8305_CONTROL_STATE, // -> DRV8305 control process
    DRV8305_DELAY_STATE,   // -> Delay state
} drv8305_sm_state_e;

typedef enum
{
    DRV8305_SM_STATUS_WARNING_REG,    // Status 0x01
    DRV8305_SM_STATUS_OV_VDS_REG,     // Status 0x02
    DRV8305_SM_STATUS_IC_FAULTS_REG,  // Status 0x03
    DRV8305_SM_STATUS_VGS_FAULTS_REG, // Status 0x04
    DRV8305_SM_STATUS_CYCLE_DELAY,    // Delay state
} drv8305_status_sm_state_e;

typedef enum
{
    // CONTOL REGISTERS WRITE STATES
    DRV8305_SM_CONTROL_HS_GATE_DRIVE_REG,          // Control 0x05: HS Gate Drive Control
    DRV8305_SM_CONTROL_LS_GATE_DRIVE_REG,          // Control 0x06: LS Gate Drive Control
    DRV8305_SM_CONTROL_GATE_DRIVE_REG,             // Control 0x07: Gate Drive Control
    DRV8305_SM_CONTROL_IC_OPERATION_REG,           // Control 0x09: IC Operation
    DRV8305_SM_CONTROL_SHUNT_AMPLIFIER_REG,        // Control 0x0A: Shunt Amplifier Control
    DRV8305_SM_CONTROL_VOLTAGE_REGULATOR_REG,      // Control 0x0B: Voltage Regulator Control
    DRV8305_SM_CONTROL_VDS_SENSE_REG,              // Control 0x0C: VDS Sense Control
    
    // CONTOL REGISTERS READ STATES
    DRV8305_SM_READ_CONTROL_HS_GATE_DRIVE_REG,     // Control 0x05: HS Gate Drive Control
    DRV8305_SM_READ_CONTROL_LS_GATE_DRIVE_REG,     // Control 0x06: LS Gate Drive Control
    DRV8305_SM_READ_CONTROL_GATE_DRIVE_REG,        // Control 0x07: Gate Drive Control
    DRV8305_SM_READ_CONTROL_IC_OPERATION_REG,      // Control 0x09: IC Operation
    DRV8305_SM_READ_CONTROL_SHUNT_AMPLIFIER_REG,   // Control 0x0A: Shunt Amplifier Control
    DRV8305_SM_READ_CONTROL_VOLTAGE_REGULATOR_REG, // Control 0x0B: Voltage Regulator Control
    DRV8305_SM_READ_CONTROL_VDS_SENSE_REG,         // Control 0x0C: VDS Sense Control
    
    DRV8305_SM_CONTROL_CYCLE_DELAY,                // Delay state    
} drv8305_control_sm_state_e;

typedef struct 
{
    uint16_t (*drv8305_spi_write_and_read_from_register_cb) (uint16_t data);
    bool     (*drv8305_get_fault_pin_status)                (void);
    void     (*drv8305_enable_io)                           (void);
    void     (*drv8305_disable_io)                          (void);
    void     (*drv8305_wake_up_io)                          (void);
    void     (*drv8305_sleep_io)                            (void);
} drv8305_hardware_low_level_cb_t;

typedef struct
{
   uint16_t                 data;
   drv8305_register_types_t type;
} drv8305_register_node_t;

typedef struct
{
    uint32_t                   cycle_time;
    uint32_t                   delay_time;

    drv8305_sm_state_e         main_state;
    drv8305_sm_state_e         next_main_state;

    drv8305_status_sm_state_e  status_state;
    drv8305_status_sm_state_e  next_status_state;

    drv8305_control_sm_state_e control_state;
    drv8305_control_sm_state_e next_control_state;
} drv8305_state_machine_t;

typedef struct
{
    void (*drv8305_warning_register_cb)    (void *self, uint16_t data);
    void (*drv8305_ov_vds_register_cb)     (void *self, uint16_t data);
    void (*drv8305_ic_faults_register_cb)  (void *self, uint16_t data);
    void (*drv8305_vgs_faults_register_cb) (void *self, uint16_t data);
} drv8305_status_register_cb_t;

typedef struct
{
    void (*drv8305_hs_gate_drive_control_register_cb)     (void *self, uint16_t data);
    void (*drv8305_ls_gate_drive_control_register_cb)     (void *self, uint16_t data);
    void (*drv8305_gate_drive_control_register_cb)        (void *self, uint16_t data);
    void (*drv8305_ic_operation_register_cb)              (void *self, uint16_t data);
    void (*drv8305_shunt_amplifier_control_register_cb)   (void *self, uint16_t data);
    void (*drv8305_voltage_regulator_control_register_cb) (void *self, uint16_t data);
    void (*drv8305_vds_sense_control_register_cb)         (void *self, uint16_t data);
} drv8305_control_register_cb_t;

typedef struct
{
    bool hs_gate_drive;
    bool ls_gate_drive;
    bool gate_drive;
    bool ic_operation;
    bool shunt_amplifier;
    bool voltage_regulator;
    bool vds_sense;
}drv8305_control_register_configuration_flag_t;

typedef struct
{
    drv8305_state_machine_t                       state;
        
    bool                                          enable_pin_status;
    bool                                          drv_wake_pin_status;

    drv8305_control_register_cb_t                 control_callbacks;
    drv8305_status_register_cb_t                  status_callbacks;
    drv8305_hardware_low_level_cb_t               hw_callbacks;

    drv8305_configuration_t                       config;    

    drv8305_register_node_t                       register_manager[DRV8305_NUMBER_OF_REGISTERS];

    drv8305_control_register_configuration_flag_t configuration_confirmation_flags;
} drv8305_user_object_t;

/**
 * @brief Initialize DRV8305 driver with hardware callbacks and configuration
 * @details Sets up the driver state machine, registers, and configuration from the
 *          configuration module. Must be called before any other API functions.
 * @param[in] self Pointer to DRV8305 user object containing callbacks and state
 * @return None
 * @note All hardware callbacks must be non-NULL before calling this function
 * @see drv8305_api_master_sm_polling
 */
DRV8305_PUBLIC void drv8305_api_initialize            (drv8305_user_object_t *self);

/**
 * @brief Main state machine polling function (non-blocking)
 * @details Executes one iteration of the DRV8305 state machine. Must be called
 *          periodically from the application main loop or timer interrupt.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Call rate should be synchronized with drv8305_api_timer() timing
 * @see drv8305_api_timer, drv8305_api_initialize
 */
DRV8305_PUBLIC void drv8305_api_master_sm_polling     (drv8305_user_object_t *self);

/**
 * @brief Increment internal cycle timer (must be called from timer interrupt)
 * @details Increments the internal timing counter used for state machine delays
 *          and polling intervals. Typically called from a periodic timer ISR.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Should be called at a fixed rate (e.g., 1ms) to maintain timing accuracy
 * @see drv8305_api_master_sm_polling
 */
DRV8305_PUBLIC void drv8305_api_timer                 (drv8305_user_object_t *self);

/**
 * @brief Enable DRV8305 IC (turn on gate drivers)
 * @details Activates the gate driver enable GPIO signal to power up the IC.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_disable
 */
DRV8305_PUBLIC void drv8305_api_ic_enable             (drv8305_user_object_t *self);

/**
 * @brief Disable DRV8305 IC (turn off gate drivers)
 * @details Deactivates the gate driver enable GPIO signal to power down the IC.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_enable
 */
DRV8305_PUBLIC void drv8305_api_ic_disable            (drv8305_user_object_t *self);

/**
 * @brief Put DRV8305 IC into sleep mode
 * @details Activates the sleep GPIO signal to put the IC into low-power mode.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_wake_up
 */
DRV8305_PUBLIC void drv8305_api_ic_sleep              (drv8305_user_object_t *self);

/**
 * @brief Wake up DRV8305 IC from sleep mode
 * @details Deactivates the sleep GPIO signal to wake the IC from low-power mode.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @see drv8305_api_ic_sleep
 */
DRV8305_PUBLIC void drv8305_api_ic_wake_up            (drv8305_user_object_t *self);

/**
 * @brief Get current DRV8305 configuration
 * @details Retrieves pointer to the active default_configuration structure
 *          containing all current control register settings (gate drive,
 *          IC operation, shunt amplifier, voltage regulator, VDS sensing).
 * @return drv8305_configuration_t* Non-NULL pointer to default configuration structure
 * @note Pointer remains valid throughout driver lifetime
 * @usage Modify configuration settings and apply with drv8305_set_configuration()
 * @see drv8305_set_configuration(), drv8305_configuration_t
 * 
 * @example
 * @code
 * // Get current configuration
 * drv8305_configuration_t* config = drv8305_get_configuration();
 * 
 * // Modify gate drive settings
 * config->hs_gate_drive.peak_current = DRV8305_CTRL05_IPEAK_2_75A;
 * config->gate_drive.pwm_mode = DRV8305_PWM_MODE_PWM_SYNC;
 * 
 * // Apply new configuration to IC
 * drv8305_set_configuration(config);
 * @endcode
 */
DRV8305_PUBLIC drv8305_configuration_t* drv8305_get_configuration(void);

/**
 * @brief Set new DRV8305 configuration
 * @details Copies provided configuration structure into the internal
 *          default_configuration, replacing all control register settings.
 *          Changes take effect after drv8305_api_confirm_configuration() call.
 * @param[in] cfg Pointer to new configuration structure to apply
 * @return None
 * @note Entire configuration is copied (memcpy operation)
 * @warning Configuration changes require drv8305_api_confirm_configuration() to take effect
 * @see drv8305_get_configuration(), drv8305_api_confirm_configuration()
 * 
 * @example
 * @code
 * // Create new configuration
 * drv8305_configuration_t new_config = {
 *     .hs_gate_drive = { .peak_current = DRV8305_CTRL05_IPEAK_1_70A },
 *     .ls_gate_drive = { .peak_current = DRV8305_CTRL06_IPEAK_1_70A },
 *     // ... other settings
 * };
 * 
 * // Apply new configuration
 * drv8305_set_configuration(&new_config);
 * drv8305_api_confirm_configuration();  // Must call to apply changes to IC
 * @endcode
 */
DRV8305_PUBLIC void drv8305_set_configuration(drv8305_configuration_t *cfg);

/**
 * @brief Confirm configuration and begin control register programming
 * @details Transitions state machine from INIT to CONTROL_STATE to start
 *          programming control registers with current configuration values.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return None
 * @note Call after drv8305_api_initialize() when configuration is ready
 * @see drv8305_api_initialize
 */
DRV8305_PUBLIC void drv8305_api_confirm_configuration(drv8305_user_object_t *self);

/**
 * @brief Check if DRV8305 configuration is confirmed
 * @details Returns the status of configuration confirmation flag.
 *          Used to verify if control register settings have been acknowledged.
 * @param[in,out] self Pointer to DRV8305 user object
 * @return true if configuration is confirmed, false otherwise
 */
DRV8305_PUBLIC bool drv8305_api_is_configuration_confirm(drv8305_user_object_t * self);

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_API_H_ */
