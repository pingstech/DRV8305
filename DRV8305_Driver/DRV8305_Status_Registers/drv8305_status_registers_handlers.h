/**
 * @file drv8305_status_registers_handlers.h
 * @brief DRV8305 Status Register Handlers - Public Interface
 * @details Declares handler functions for all 4 DRV8305 status registers
 *          providing fault and warning flag processing.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module defines callbacks for status register read operations:
 *   - Register 0x01: Warning Register (temperature, supply, watchdog)
 *   - Register 0x02: OV/VDS Faults (overcurrent on shunt and MOSFETs)
 *   - Register 0x03: IC Faults (charge pump, supply, thermal shutdown)
 *   - Register 0x04: VGS Faults (gate drive circuit faults on 6 MOSFETs)
 * 
 * @callback_signature
 * void handler(void *self, uint16_t data)
 *   - self: Pointer to DRV8305 user object (for future expansion)
 *   - data: Register contents containing fault/warning flags
 * 
 * @datasheet_reference
 * DRV8305-Q1 Status Registers (Pages 38-39, Table 10-13)
 */

#ifndef DRV8305_STATUS_REGISTERS_HANDLERS_H_
#define DRV8305_STATUS_REGISTERS_HANDLERS_H_

#ifdef __cplusplus
extern "C" {
#endif

DRV8305_PUBLIC void drv8305_warning_register_handler    (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_ov_vds_register_handler     (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_ic_faults_register_handler  (void *self, uint16_t data);
DRV8305_PUBLIC void drv8305_vgs_faults_register_handler (void *self, uint16_t data);

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_STATUS_REGISTERS_HANDLERS_H_ */
