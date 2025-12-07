/**
 * @file drv8305_app.c
 * @brief DRV8305 Application Layer - Hardware Implementation
 * @details Implements platform-specific hardware callbacks and application-level
 *          wrapper functions for the DRV8305 driver on TI C2000 DSP platform.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This implementation file provides:
 *   - Hardware I/O callbacks (GPIO control for EN_GATE, DRV_WAKE)
 *   - SPI communication callbacks (SPIA transmit/receive)
 *   - Status register callback wrappers (warning, OV/VDS, IC faults, VGS faults)
 *   - Control register callback wrappers (gate drive, IC operation, sensing, etc.)
 *   - Application wrapper functions (initialize, polling, timer, motor start/stop)
 *   - Global DRV8305 user object instantiation and configuration
 * 
 * @spi_interface
 * SPIA Configuration (Device.h setup):
 *   - 16-bit SPI word length
 *   - Blocking FIFO read/write operations
 *   - Master mode, clock polarity/phase per DRV8305 datasheet
 * 
 * @gpio_pins
 * EN_GATE:  Gate driver enable (active high)
 * DRV_WAKE: Sleep mode control (high=awake, low=sleep)
 */

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "drv8305_macros.h"
#include "drv8305_status_registers_handlers.h"
#include "drv8305_control_registers_handlers.h"
#include "drv8305_api.h"
#include "drv8305_app.h"

DRV8305_PRIVATE void     hardware_drv8305_io_disable_callback       (void);
DRV8305_PRIVATE void     hardware_drv8305_io_enable_callback        (void);
DRV8305_PRIVATE void     hardware_drv8305_sleep_io_enable_callback  (void);
DRV8305_PRIVATE void     hardware_drv8305_sleep_io_disable_callback (void);
DRV8305_PRIVATE void     hardware_spi_transmit_callback             (uint16_t data);
DRV8305_PRIVATE uint16_t hardware_spi_receive_callback              (void);

DRV8305_PRIVATE void     drv8305_warning_callback                   (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_ov_vds_callback                    (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_ic_faults_callback                 (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_vgs_faults_callback                (void *self, uint16_t data);

DRV8305_PRIVATE void     drv8305_hs_gate_drive_callback             (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_ls_gate_drive_callback             (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_gate_drive_callback                (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_ic_operation_callback              (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_shunt_amplifier_callback           (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_voltage_regulator_callback         (void *self, uint16_t data);
DRV8305_PRIVATE void     drv8305_vds_sens_callback                  (void *self, uint16_t data);

DRV8305_PRIVATE drv8305_user_object_t user_drv8305_obj =
{
    .hw_callbacks =
    {
        .drv8305_disable_io      = hardware_drv8305_io_disable_callback,
        .drv8305_enable_io       = hardware_drv8305_io_enable_callback,
        .drv8305_sleep_io        = hardware_drv8305_sleep_io_disable_callback,
        .drv8305_wake_up_io      = hardware_drv8305_sleep_io_enable_callback,
        .drv8305_spi_transmit_cb = hardware_spi_transmit_callback,
        .drv8305_spi_receive_cb  = hardware_spi_receive_callback
    },

    .status_callbacks = 
    {
        .drv8305_warning_register_cb    = drv8305_warning_callback,
        .drv8305_ov_vds_register_cb     = drv8305_ov_vds_callback,
        .drv8305_ic_faults_register_cb  = drv8305_ic_faults_callback,
        .drv8305_vgs_faults_register_cb = drv8305_vgs_faults_callback
    },
    
    .control_callbacks =
    {
        .drv8305_hs_gate_drive_control_register_cb     = drv8305_hs_gate_drive_callback,
        .drv8305_ls_gate_drive_control_register_cb     = drv8305_ls_gate_drive_callback,
        .drv8305_gate_drive_control_register_cb        = drv8305_gate_drive_callback,
        .drv8305_ic_operation_register_cb              = drv8305_ic_operation_callback,
        .drv8305_shunt_amplifier_control_register_cb   = drv8305_shunt_amplifier_callback,
        .drv8305_voltage_regulator_control_register_cb = drv8305_voltage_regulator_callback,
        .drv8305_vds_sense_control_register_cb         = drv8305_vds_sens_callback
    }
};

DRV8305_PUBLIC void drv8305_initialize(void)
{
    drv8305_initialize(&user_drv8305_obj);
}

/**
 * @brief Initialize DRV8305 driver with application configuration
 * @details Application-level initialization wrapper. Sets up all hardware callbacks,
 *          loads default configuration, and initializes driver state machine.
 * @return None
 * @note Must be called once during startup before drv8305_polling()
 * @see drv8305_polling, drv8305_timer
 */
DRV8305_PUBLIC void drv8305_initialize(void)
 * @brief Main polling function for DRV8305 driver
 * @details Application-level polling wrapper executing one state machine cycle.
 *          Call from main application loop at regular intervals.
 * @return None
 * @note Must be synchronized with drv8305_timer() call rate
 * @see drv8305_timer, drv8305_initialize
 */
DRV8305_PUBLIC void drv8305_polling(void)
{
    drv8305_api_master_sm_polling(&user_drv8305_obj);
}

/**
 * @brief Increment driver timer (call from periodic interrupt)
 * @details Application-level timer wrapper. Call from fixed-rate timer interrupt
 *          (typically 1ms) to maintain state machine timing accuracy.
 * @return None
 * @note Should be called at synchronized rate with drv8305_polling()
 * @see drv8305_polling
 */
DRV8305_PUBLIC void drv8305_timer(void)
{
    drv8305_api_timer(&user_drv8305_obj);
}

/**
 * @brief Stop motor (disable DRV8305 gate drivers)
 * @details Application-level convenience function to immediately disable all gate drivers.
 *          Halts PWM output to MOSFETs causing motor to coast/freewheel.
 * @return None
 * @see drv8305_motor_run, drv8305_api_ic_disable
 */
DRV8305_PUBLIC void drv8305_motor_stop(void)
{
    drv8305_api_ic_disable(&user_drv8305_obj);
}

/**
 * @brief Start motor (enable DRV8305 gate drivers)
 * @details Application-level convenience function to enable gate drivers after
 *          driver initialization and configuration is complete.
 * @return None
 * @see drv8305_motor_stop, drv8305_api_ic_enable
 */
DRV8305_PUBLIC void drv8305_motor_run(void)
{
    drv8305_api_ic_enable(&user_drv8305_obj);
}

/**
 * @brief Confirm DRV8305 configuration completion (application wrapper)
 * @details Calls the core API function to confirm configuration.
 *          Sends acknowledgment of control register programming to DRV8305 IC.
 *          Part of initialization sequence after all registers are configured.
 * @return None
 * @note Wrapper function using global user_drv8305_obj instance
 * @see drv8305_api_confirm_configuration(), drv8305_initialize()
 * @internal Uses blocking API call - safe to call from main context
 */
DRV8305_PUBLIC void drv8305_confirm_configuration(void)
{
    drv8305_api_confirm_configuration(&user_drv8305_obj);
}

// ============================================================================
// HARDWARE I/O CALLBACKS - Implementation-specific GPIO and SPI control
// ============================================================================

/**
 * @brief Disable DRV8305 gate drivers via GPIO (hardware callback)
 * @details Sets EN_GATE GPIO to low, disabling all gate driver outputs.
 * @return None
 * @note Internal callback - mapped to hw_callbacks.drv8305_disable_io
 */
DRV8305_PRIVATE void hardware_drv8305_io_disable_callback(void)
{
    GPIO_writePin(EN_GATE, 0);
}

/**
 * @brief Enable DRV8305 gate drivers via GPIO (hardware callback)
 * @details Sets EN_GATE GPIO to high, enabling all gate driver outputs.
 * @return None
 * @note Internal callback - mapped to hw_callbacks.drv8305_enable_io
 */
DRV8305_PRIVATE void hardware_drv8305_io_enable_callback(void)
{
    GPIO_writePin(EN_GATE, 1);
}

/**
 * @brief Wake up DRV8305 from sleep mode (hardware callback)
 * @details Sets DRV_WAKE GPIO to high, releasing sleep mode and enabling normal operation.
 * @return None
 * @note Internal callback - mapped to hw_callbacks.drv8305_wake_up_io
 */
DRV8305_PRIVATE void hardware_drv8305_sleep_io_enable_callback(void)
{
    GPIO_writePin(DRV_WAKE, 1);
}

/**
 * @brief Put DRV8305 into sleep mode (hardware callback)
 * @details Sets DRV_WAKE GPIO to low, placing IC into low-power sleep state.
 * @return None
 * @note Internal callback - mapped to hw_callbacks.drv8305_sleep_io
 */
DRV8305_PRIVATE void hardware_drv8305_sleep_io_disable_callback(void)
{    
    GPIO_writePin(DRV_WAKE, 0);
}

/**
 * @brief Transmit SPI command packet (hardware callback)
 * @details Sends 16-bit command packet to DRV8305 via SPIA in blocking mode.
 * @param[in] data SPI packet data to transmit
 * @return None
 * @note Internal callback - mapped to hw_callbacks.drv8305_spi_transmit_cb
 */
DRV8305_PRIVATE void hardware_spi_transmit_callback(uint16_t data)
{
    SPI_writeDataBlockingFIFO(SPIA_BASE, data);
}

/**
 * @brief Receive SPI response packet (hardware callback)
 * @details Reads 16-bit response packet from DRV8305 via SPIA in blocking mode.
 * @return uint16_t SPI response packet data
 * @note Internal callback - mapped to hw_callbacks.drv8305_spi_receive_cb
 */
DRV8305_PRIVATE uint16_t hardware_spi_receive_callback(void)
{
    return SPI_readDataBlockingFIFO(SPIA_BASE);
}

// ============================================================================
// STATUS REGISTER CALLBACKS - Wrapper functions called during status polling
// ============================================================================

/**
 * @brief Status Register 0x01 (Warning) callback wrapper (internal)
 * @details Adapter function calling register handler with only data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data Warning register contents
 * @return None
 * @note Internal callback - implementation-specific warning handling
 */
DRV8305_PRIVATE void drv8305_warning_callback(void *self, uint16_t data)
{
    drv8305_warning_register_handler(data);
}

/**
 * @brief Status Register 0x02 (OV/VDS Faults) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data OV/VDS Faults register contents
 * @return None
 */
DRV8305_PRIVATE void drv8305_ov_vds_callback(void *self, uint16_t data)
{
    drv8305_ov_vds_register_handler(data);
}

/**
 * @brief Status Register 0x03 (IC Faults) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data IC Faults register contents
 * @return None
 */
DRV8305_PRIVATE void drv8305_ic_faults_callback(void *self, uint16_t data)
{
    drv8305_ic_faults_register_handler(data);
}

/**
 * @brief Status Register 0x04 (VGS Faults) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data VGS Faults register contents
 * @return None
 */
DRV8305_PRIVATE void drv8305_vgs_faults_callback(void *self, uint16_t data)
{
    drv8305_vgs_faults_register_handler(data);
}

// ============================================================================
// CONTROL REGISTER CALLBACKS - Wrapper functions called during control programming
// ============================================================================

/**
 * @brief Control Register 0x05 (HS Gate Drive) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data HS Gate Drive register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_hs_gate_drive_callback(void *self, uint16_t data)
{
    drv8305_hs_gate_driver_register_handler(data);
}

/**
 * @brief Control Register 0x06 (LS Gate Drive) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data LS Gate Drive register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_ls_gate_drive_callback(void *self, uint16_t data)
{
    drv8305_ls_gate_drive_register_handler(data);
}

/**
 * @brief Control Register 0x07 (Gate Drive Control) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data Gate Drive Control register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_gate_drive_callback(void *self, uint16_t data)
{
    drv8305_gate_drive_register_handler(data);
}

/**
 * @brief Control Register 0x09 (IC Operation) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data IC Operation register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_ic_operation_callback(void *self, uint16_t data)
{
    drv8305_ic_operation_register_handler(data);
}

/**
 * @brief Control Register 0x0A (Shunt Amplifier) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data Shunt Amplifier Control register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_shunt_amplifier_callback(void *self, uint16_t data)
{
    drv8305_shunt_amplifier_register_handler(data);
}

/**
 * @brief Control Register 0x0B (Voltage Regulator) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data Voltage Regulator Control register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_voltage_regulator_callback(void *self, uint16_t data)
{
    drv8305_voltage_regulator_register_handler(data);
}

/**
 * @brief Control Register 0x0C (VDS Sense Control) callback wrapper (internal)
 * @details Adapter function calling register handler with data parameter.
 * @param[in] self Pointer to DRV8305 user object
 * @param[in] data VDS Sense Control register contents (echo)
 * @return None
 */
DRV8305_PRIVATE void drv8305_vds_sens_callback(void *self, uint16_t data)
{
    drv8305_vds_sense_register_handler(data);
}
