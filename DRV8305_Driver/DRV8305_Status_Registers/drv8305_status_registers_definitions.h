/**
 * @file drv8305_status_registers_definitions.h
 * @brief DRV8305 Status Registers - Bit Definitions and Constants
 * @details Defines bit masks for all 4 DRV8305 status registers (read-only)
 *          including temperature warnings, fault flags, and monitoring status.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides symbolic definitions for status register bits:
 *   - Register 0x01: Warning register (11 bits: temperature, supply, watchdog)
 *   - Register 0x02: OV/VDS Faults (11 bits: shunt OCP, MOSFET VDS overcurrent)
 *   - Register 0x03: IC Faults (11 bits: charge pump, supply, thermal, watchdog)
 *   - Register 0x04: VGS Faults (11 bits: gate drive faults on 6 MOSFETs)
 * 
 * @naming_convention
 * DRV8305_[REGISTER_NAME]_[BIT_DESCRIPTION]
 *   REGISTER_NAME: WARN, VDS, IC, VGS
 *   BIT_DESCRIPTION: Descriptive flag name (e.g., OTW, PVDD_OVFL, HA, LC)
 * 
 * @datasheet_reference
 * DRV8305-Q1 Status Registers (Pages 38-39, Table 10-13)
 */

#ifndef DRV8305_STATUS_REGISTERS_DEFINITIONS_H_
#define DRV8305_STATUS_REGISTERS_DEFINITIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * STATUS REGISTERS (Read-Only)
 * Datasheet Reference: Table 10-13, Pages 38-39
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Register 0x01: Warning & Watchdog Reset
 * Note: Reading this register resets the MCU watchdog timer
 * ------------------------------------------------------------------------- */
#define DRV8305_WARN_OTW            (1U << 0)   // Overtemperature warning
#define DRV8305_WARN_TEMP_FLAG3     (1U << 1)   // Temperature flag 3 (~135°C)
#define DRV8305_WARN_TEMP_FLAG2     (1U << 2)   // Temperature flag 2 (~125°C)
#define DRV8305_WARN_TEMP_FLAG1     (1U << 3)   // Temperature flag 1 (~105°C)
#define DRV8305_WARN_VCPH_UVFL      (1U << 4)   // Charge pump undervoltage flag warning
#define DRV8305_WARN_VDS_STATUS     (1U << 5)   // VDS overcurrent monitor (OR of all VDS monitors)
#define DRV8305_WARN_PVDD_OVFL      (1U << 6)   // PVDD overvoltage flag warning
#define DRV8305_WARN_PVDD_UVFL      (1U << 7)   // PVDD undervoltage flag warning
#define DRV8305_WARN_TEMP_FLAG4     (1U << 8)   // Temperature flag 4 (~175°C)
#define DRV8305_WARN_RSVD           (1U << 9)   // Reserved
#define DRV8305_WARN_FAULT          (1U << 10)  // Global fault indication

/* -------------------------------------------------------------------------
 * Register 0x02: OV/VDS Faults
 * ------------------------------------------------------------------------- */
#define DRV8305_VDS_SNS_A_OCP       (1U << 0)   // Sense A overcurrent fault
#define DRV8305_VDS_SNS_B_OCP       (1U << 1)   // Sense B overcurrent fault
#define DRV8305_VDS_SNS_C_OCP       (1U << 2)   // Sense C overcurrent fault
#define DRV8305_VDS_RSVD_3          (1U << 3)   // Reserved
#define DRV8305_VDS_RSVD_4          (1U << 4)   // Reserved
#define DRV8305_VDS_LC              (1U << 5)   // VDS overcurrent fault for low-side MOSFET C
#define DRV8305_VDS_HC              (1U << 6)   // VDS overcurrent fault for high-side MOSFET C
#define DRV8305_VDS_LB              (1U << 7)   // VDS overcurrent fault for low-side MOSFET B
#define DRV8305_VDS_HB              (1U << 8)   // VDS overcurrent fault for high-side MOSFET B
#define DRV8305_VDS_LA              (1U << 9)   // VDS overcurrent fault for low-side MOSFET A
#define DRV8305_VDS_HA              (1U << 10)  // VDS overcurrent fault for high-side MOSFET A

/* -------------------------------------------------------------------------
 * Register 0x03: IC Faults
 * ------------------------------------------------------------------------- */
#define DRV8305_IC_VCPH_OVLO_ABS    (1U << 0)   // Charge pump absolute overvoltage fault
#define DRV8305_IC_VCPH_OVLO        (1U << 1)   // Charge pump overvoltage fault (relative to PVDD)
#define DRV8305_IC_VCPH_UVLO2       (1U << 2)   // Charge pump undervoltage 2 fault
#define DRV8305_IC_RSVD_3           (1U << 3)   // Reserved
#define DRV8305_IC_VCP_LSD_UVLO2    (1U << 4)   // Low-side gate supply undervoltage fault
#define DRV8305_IC_AVDD_UVLO        (1U << 5)   // AVDD undervoltage fault
#define DRV8305_IC_VREG_UV          (1U << 6)   // VREG undervoltage fault
#define DRV8305_IC_RSVD_7           (1U << 7)   // Reserved
#define DRV8305_IC_OTSD             (1U << 8)   // Overtemperature shutdown fault
#define DRV8305_IC_WD_FAULT         (1U << 9)   // Watchdog fault
#define DRV8305_IC_PVDD_UVLO2       (1U << 10)  // PVDD undervoltage 2 fault

/* -------------------------------------------------------------------------
 * Register 0x04: VGS Faults
 * ------------------------------------------------------------------------- */
#define DRV8305_VGS_RSVD_0          (1U << 0)   // Reserved
#define DRV8305_VGS_RSVD_1          (1U << 1)   // Reserved
#define DRV8305_VGS_RSVD_2          (1U << 2)   // Reserved
#define DRV8305_VGS_RSVD_3          (1U << 3)   // Reserved
#define DRV8305_VGS_RSVD_4          (1U << 4)   // Reserved
#define DRV8305_VGS_LC              (1U << 5)   // VGS gate drive fault for low-side MOSFET C
#define DRV8305_VGS_HC              (1U << 6)   // VGS gate drive fault for high-side MOSFET C
#define DRV8305_VGS_LB              (1U << 7)   // VGS gate drive fault for low-side MOSFET B
#define DRV8305_VGS_HB              (1U << 8)   // VGS gate drive fault for high-side MOSFET B
#define DRV8305_VGS_LA              (1U << 9)   // VGS gate drive fault for low-side MOSFET A
#define DRV8305_VGS_HA              (1U << 10)  // VGS gate drive fault for high-side MOSFET A

#ifdef __cplusplus
}
#endif

#endif /* DRV8305_STATUS_REGISTERS_DEFINITIONS_H_ */
