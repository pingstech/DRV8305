/**
 * @file drv8305_macros.h
 * @brief DRV8305 Driver - Global Macro Definitions
 * @details Defines visibility modifiers, timing constants, and array indexing
 *          macros used throughout the DRV8305 driver.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides:
 *   - Visibility control: DRV8305_PRIVATE (static), DRV8305_PUBLIC
 *   - Timing constants: Register switching delay, status polling interval
 *   - Array indexing: Status/control register array positions
 *   - Utility macros: Callback safety checks
 * 
 * @timing_constants
 * DRV8305_REGISTER_SWITCH_DELAY_MS: Delay between consecutive SPI register operations (50ms)
 * DRV8305_STATUS_POLLING_INTERVAL_MS: Interval for periodic status register polling (250ms)
 * DRV8305_NUMBER_OF_REGISTERS: Total registers managed (11: 4 status + 7 control)
 * 
 * @array_indexing
 * Register index constants for register_manager[] array:
 *   0-3:  Status registers (0x01, 0x02, 0x03, 0x04)
 *   4-10: Control registers (0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C)
 */

#ifndef DRV8305_MACROS_H_
#define DRV8305_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DRV8305_PRIVATE static
#define DRV8305_PUBLIC

/** @brief Total number of managed registers (4 status + 7 control) */
#define DRV8305_NUMBER_OF_REGISTERS         (int)11
/** @brief Interval for periodic status register polling in milliseconds */
#define DRV8305_STATUS_POLLING_INTERVAL_MS  (int)250
/** @brief Delay between consecutive SPI register operations in milliseconds */
#define DRV8305_REGISTER_SWITCH_DELAY_MS    (int)50

/** @brief Array index for Status Register 0x01 (Warning) */
#define DRV8305_STATUS_01_ARRAY_INDEX    0U
/** @brief Array index for Status Register 0x02 (OV/VDS Faults) */
#define DRV8305_STATUS_02_ARRAY_INDEX    1U
/** @brief Array index for Status Register 0x03 (IC Faults) */
#define DRV8305_STATUS_03_ARRAY_INDEX    2U
/** @brief Array index for Status Register 0x04 (VGS Faults) */
#define DRV8305_STATUS_04_ARRAY_INDEX    3U
/** @brief Array index for Control Register 0x05 (HS Gate Drive) */
#define DRV8305_CONTROL_05_ARRAY_INDEX   4U
/** @brief Array index for Control Register 0x06 (LS Gate Drive) */
#define DRV8305_CONTROL_06_ARRAY_INDEX   5U
/** @brief Array index for Control Register 0x07 (Gate Drive) */
#define DRV8305_CONTROL_07_ARRAY_INDEX   6U
/** @brief Array index for Control Register 0x09 (IC Operation) */
#define DRV8305_CONTROL_09_ARRAY_INDEX   7U
/** @brief Array index for Control Register 0x0A (Shunt Amplifier) */
#define DRV8305_CONTROL_0A_ARRAY_INDEX   8U
/** @brief Array index for Control Register 0x0B (Voltage Regulator) */
#define DRV8305_CONTROL_0B_ARRAY_INDEX   9U
/** @brief Array index for Control Register 0x0C (VDS Sense) */
#define DRV8305_CONTROL_0C_ARRAY_INDEX   10U

/** @brief Safe callback invocation macro - only calls if callback is non-NULL */
#define DRV8305_NULL_CALLBACK_SAFETY(callback)  do { if((callback) != NULL) { (callback)(); } } while(0)

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_MACROS_H_ */
