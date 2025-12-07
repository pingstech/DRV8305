/**
 * @file drv8305_register_map.h
 * @brief DRV8305 Register Address Definitions
 * @details Defines memory-mapped register addresses for all DRV8305 control
 *          and status registers accessible via SPI protocol.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides register address constants:
 *   - Status Registers (Read-Only): 0x01, 0x02, 0x03, 0x04
 *   - Control Registers (Read/Write): 0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C
 *   - Used as symbolic names in SPI packet construction
 * 
 * @register_layout
 * Status Registers (Read-Only):
 *   0x01: Warning Status (temperature, supply, watchdog)
 *   0x02: OV/VDS Faults (overcurrent monitoring)
 *   0x03: IC Faults (internal health)
 *   0x04: VGS Faults (gate drive faults)
 * 
 * Control Registers (Read/Write):
 *   0x05: HS Gate Drive Control
 *   0x06: LS Gate Drive Control
 *   0x07: Gate Drive Control
 *   0x09: IC Operation
 *   0x0A: Shunt Amplifier Control
 *   0x0B: Voltage Regulator Control
 *   0x0C: VDS Sense Control
 * 
 * @datasheet_reference
 * DRV8305-Q1 SPI Communication (Pages 37-44)
 */

#ifndef DRV8305_REGISTER_MAP_H_
#define DRV8305_REGISTER_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/// @name Status Registers (Read-Only)
/// @{
/** @brief Status 01 Register Address: Warning Status */
#define DRV8305_STATUS_01_REG_ADDR      (uint16_t)(0x01)
/** @brief Status 02 Register Address: OV_VDS Faults */
#define DRV8305_STATUS_02_REG_ADDR      (uint16_t)(0x02)
/** @brief Status 03 Register Address: IC Faults */
#define DRV8305_STATUS_03_REG_ADDR      (uint16_t)(0x03)
/** @brief Status 04 Register Address: VGS Faults */
#define DRV8305_STATUS_04_REG_ADDR      (uint16_t)(0x04)
/// @}

/// @name Control Registers (Read/Write)
/// @{
/** @brief Control 05 Register Address: HS Gate Drive Control */
#define DRV8305_CONTROL_05_REG_ADDR     (uint16_t)(0x05)
/** @brief Control 06 Register Address: LS Gate Drive Control */
#define DRV8305_CONTROL_06_REG_ADDR     (uint16_t)(0x06)
/** @brief Control 07 Register Address: Gate Drive Control */
#define DRV8305_CONTROL_07_REG_ADDR     (uint16_t)(0x07)
/** @brief Control 09 Register Address: IC Operation */
#define DRV8305_CONTROL_09_REG_ADDR     (uint16_t)(0x09)
/** @brief Control 0A Register Address: Shunt Amplifier Control */
#define DRV8305_CONTROL_0A_REG_ADDR     (uint16_t)(0x0A)
/** @brief Control 0B Register Address: Voltage Regulator Control */
#define DRV8305_CONTROL_0B_REG_ADDR     (uint16_t)(0x0B)
/** @brief Control 0C Register Address: VDS Sense Control */
#define DRV8305_CONTROL_0C_REG_ADDR     (uint16_t)(0x0C)
/// @}

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_REGISTER_MAP_H_ */
