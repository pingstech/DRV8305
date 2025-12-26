/**
 * @file drv8305_control_registers_handlers.h
 * @brief DRV8305 Control Register Handlers - Public Interface
 * @details Declares handler functions for all 7 DRV8305 control registers
 *          providing register-specific processing and configuration acknowledgment.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module defines callbacks for control register write operations:
 *   - Register 0x05: HS Gate Drive Control (peak current, drive time)
 *   - Register 0x06: LS Gate Drive Control (peak current, drive time)
 *   - Register 0x07: Gate Drive Control (charge pump, PWM mode, dead time)
 *   - Register 0x09: IC Operation (watchdog, faults, sleep)
 *   - Register 0x0A: Shunt Amplifier Control (current sense gain, DC calibration)
 *   - Register 0x0B: Voltage Regulator Control (VREF, sleep delay, UV threshold)
 *   - Register 0x0C: VDS Sense Control (threshold, response mode)
 * 
 * @callback_signature
 * void handler(void *self, uint16_t data)
 *   - self: Pointer to DRV8305 user object (for future expansion)
 *   - data: Echo of written register value from SPI response
 */

#ifndef DRV8305_CONTROL_REGISTERS_HANDLERS_H_
#define DRV8305_CONTROL_REGISTERS_HANDLERS_H_

#ifdef __cplusplus
extern "C" {
#endif

DRV8305_PUBLIC void drv8305_hs_gate_drive_register_handler     (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_ls_gate_drive_register_handler     (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_gate_drive_register_handler        (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_ic_operation_register_handler      (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_shunt_amplifier_register_handler   (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_voltage_regulator_register_handler (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_vds_sense_register_handler         (void *self, uint16_t data);

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_CONTROL_REGISTERS_HANDLERS_H_ */
