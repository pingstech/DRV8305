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
 * DRV8305_STANDARD_TASK_DELAY_TIMEOUT: Standard task delay timeout for state machine transitions (50ms)
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

#define DRV8305_PUBLIC
#define DRV8305_PRIVATE static

/** @brief Total number of managed registers (4 status + 7 control)                  */
#define DRV8305_NUMBER_OF_REGISTERS         (int)11
/** @brief Interval for periodic status register polling in milliseconds             */
#define DRV8305_STATUS_POLLING_INTERVAL_MS  (int)250
/** @brief Standard task delay timeout for state machine transitions in milliseconds */
#define DRV8305_STANDARD_TASK_DELAY_TIMEOUT (int)500
/** @brief Delay between consecutive SPI register operations in milliseconds         */
#define DRV8305_REGISTER_SWITCH_DELAY_MS    (int)50

/** @brief Array index for Status Register 0x01 (Warning)               */
#define DRV8305_STATUS_01_ARRAY_INDEX    0U
/** @brief Array index for Status Register 0x02 (OV/VDS Faults)         */
#define DRV8305_STATUS_02_ARRAY_INDEX    1U
/** @brief Array index for Status Register 0x03 (IC Faults)             */
#define DRV8305_STATUS_03_ARRAY_INDEX    2U
/** @brief Array index for Status Register 0x04 (VGS Faults)            */
#define DRV8305_STATUS_04_ARRAY_INDEX    3U
/** @brief Array index for Control Register 0x05 (HS Gate Drive)        */
#define DRV8305_CONTROL_05_ARRAY_INDEX   4U
/** @brief Array index for Control Register 0x06 (LS Gate Drive)        */
#define DRV8305_CONTROL_06_ARRAY_INDEX   5U
/** @brief Array index for Control Register 0x07 (Gate Drive)           */
#define DRV8305_CONTROL_07_ARRAY_INDEX   6U
/** @brief Array index for Control Register 0x09 (IC Operation)         */
#define DRV8305_CONTROL_09_ARRAY_INDEX   7U
/** @brief Array index for Control Register 0x0A (Shunt Amplifier)      */
#define DRV8305_CONTROL_0A_ARRAY_INDEX   8U
/** @brief Array index for Control Register 0x0B (Voltage Regulator)    */
#define DRV8305_CONTROL_0B_ARRAY_INDEX   9U
/** @brief Array index for Control Register 0x0C (VDS Sense)            */
#define DRV8305_CONTROL_0C_ARRAY_INDEX   10U

/** @brief Control register 05 and 06 masks **/
#define DRV8305_CTRL05_CTRL06_TDRIVE_MASK   (0x03u << 8)  /* bits 9:8 */
#define DRV8305_CTRL05_CTRL06_ISINK_MASK    (0x0Fu << 4)  /* bits 7:4 */
#define DRV8305_CTRL05_CTRL06_ISOURCE_MASK  (0x0Fu << 0)  /* bits 3:0 */

/** @brief Control register 07 masks **/
#define DRV8305_CTRL07_VCPH_FREQ_MASK       (0x01u << 10) /* bits 10  */
#define DRV8305_CTRL07_COMM_OPTION_MASK     (0x01u << 9)  /* bits 9   */
#define DRV8305_CTRL07_PWM_MODE_MASK        (0x03u << 7)  /* bits 8:7 */  
#define DRV8305_CTRL07_DEAD_TIME_MASK       (0x07u << 4)  /* bits 6:4 */
#define DRV8305_CTRL07_TBLANK_MASK          (0x03u << 2)  /* bits 3:2 */
#define DRV8305_CTRL07_TVDS_MASK            (0x03u << 0)  /* bits 1:0 */

/** @brief Control register 09 masks **/
#define DRV8305_CTRL09_FLIP_OTSD_MASK       (0x01u << 10) /* bit 10   */
#define DRV8305_CTRL09_DIS_PVDD_UVLO2_MASK  (0x01u << 9)  /* bit 9    */
#define DRV8305_CTRL09_DIS_GDRV_FAULT_MASK  (0x01u << 8)  /* bit 8    */
#define DRV8305_CTRL09_EN_SNS_CLAMP_MASK    (0x01u << 7)  /* bit 7    */
#define DRV8305_CTRL09_WD_DLY_MASK          (0x03u << 5)  /* bits 6:5 */
#define DRV8305_CTRL09_DIS_SNS_OCP_MASK     (0x01u << 4)  /* bit 4    */
#define DRV8305_CTRL09_WD_EN_MASK           (0x01u << 3)  /* bit 3    */
#define DRV8305_CTRL09_SLEEP_MASK           (0x01u << 2)  /* bit 2    */
#define DRV8305_CTRL09_CLR_FLTS_MASK        (0x01u << 1)  /* bit 1    */
#define DRV8305_CTRL09_SET_VCPH_UV_MASK     (0x01u << 0)  /* bit 0    */

/** @brief Control register 0A masks **/
#define DRV8305_CTRL0A_DC_CAL_CH3_MASK      (0x01u << 10) /* bit 10   */
#define DRV8305_CTRL0A_DC_CAL_CH2_MASK      (0x01u << 9)  /* bit 9    */
#define DRV8305_CTRL0A_DC_CAL_CH1_MASK      (0x01u << 8)  /* bit 8    */
#define DRV8305_CTRL0A_CS_BLANK_MASK        (0x03u << 6)  /* bits 7:6 */
#define DRV8305_CTRL0A_GAIN_CH3_MASK        (0x07u << 4)  /* bits 5:4 */
#define DRV8305_CTRL0A_GAIN_CH2_MASK        (0x07u << 2)  /* bits 3:2 */
#define DRV8305_CTRL0A_GAIN_CH1_MASK        (0x07u << 0)  /* bits 1:0 */

/** @brief Control register 0B masks **/
#define DRV8305_CTRL0B_VREF_SCALE_MASK      (0x03u << 8)  /* bits 9:8 */
#define DRV8305_CTRL0B_SLEEP_DELAY_MASK     (0x03u << 3)  /* bits 4:3 */
#define DRV8305_CTRL0B_DIS_VREG_PWRGD_MASK  (0x01u << 2)  /* bit 2    */
#define DRV8305_CTRL0B_VREG_UV_LEVEL_MASK   (0x03u << 0)  /* bits 1:0 */

/** @brief Control register 0C masks **/
#define DRV8305_CTRL0C_VDS_LEVEL_MASK      (0x1Fu << 3)   /* bits 7:3 */
#define DRV8305_CTRL0C_VDS_MODE_MASK       (0x07u << 0)   /* bits 2:0 */

/** @brief Safe callback invocation macro - only calls if callback is non-NULL */
#define DRV8305_NULL_CALLBACK_SAFETY(callback)  do { if((callback) != NULL) { (callback)(); } } while(0)

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_MACROS_H_ */
