/**
 * @file drv8305_control_registers_definitions.h
 * @brief DRV8305 Control Registers - Parameter Types and Packing Macros
 * @details Defines enums, structs, and packing macros for all 7 DRV8305 control
 *          registers enabling configuration of gate drive, IC operation, and sensing.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides type-safe configuration for control registers:
 *   - Enumerations for each register parameter (drive currents, gate timing, etc.)
 *   - Typedef structures aggregating related parameters
 *   - DRV8305_PACK_CTRLxx macros for SPI packet creation
 *   - Register bit field definitions with datasheet values
 * 
 * @register_coverage
 *   - Registers 0x05/0x06: HS/LS Gate Drive Control (drive times, currents)
 *   - Register 0x07: Gate Drive Control (charge pump, PWM mode, dead time, VDS blanking)
 *   - Register 0x09: IC Operation (watchdog, undervoltage, fault masks, sleep)
 *   - Register 0x0A: Shunt Amplifier Control (DC calibration, gain per channel)
 *   - Register 0x0B: Voltage Regulator Control (VREF scaling, sleep delay, UV level)
 *   - Register 0x0C: VDS Sense Control (threshold levels, response mode)
 * 
 * @spi_encoding
 * Each register 0-10 bits map to specific parameter fields
 * Packing macros handle bit shifting and masking for correct SPI format
 * 
 * @datasheet_reference
 * DRV8305-Q1 Control Registers (Pages 40-44, Table 14-20)
 */

#ifndef DRV8305_CONTROL_REGISTERS_DEFINITIONS_H_
#define DRV8305_CONTROL_REGISTERS_DEFINITIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ============================================================================
 * CONTROL REGISTERS (Read/Write)
 * Datasheet Reference: Table 14-20, Pages 40-44
 * ============================================================================ */

/* -------------------------------------------------------------------------
 * Register 0x05 & 0x06: Gate Drive Control Definitions
 * Datasheet: Table 14 (HS), Table 15 (LS)
 * ------------------------------------------------------------------------- */

/**
 * @brief Peak current drive time (TDRIVE) settings
 * Datasheet Reference: Table 14, Bits 9:8
 */
typedef enum 
{
    DRV8305_TDRIVE_220NS  = 0x0U,  // 220 ns
    DRV8305_TDRIVE_440NS  = 0x1U,  // 440 ns
    DRV8305_TDRIVE_880NS  = 0x2U,  // 880 ns
    DRV8305_TDRIVE_1780NS = 0x3U   // 1780 ns (Default)
} drv8305_tdrive_t;

/**
 * @brief High-side peak SINK current settings (IDRIVEN_HS)
 * Datasheet Reference: Table 14, Bits 7:4
 */
typedef enum 
{
    DRV8305_ISINK_20MA   = 0x0U,  // 20 mA
    DRV8305_ISINK_30MA   = 0x1U,  // 30 mA
    DRV8305_ISINK_40MA   = 0x2U,  // 40 mA
    DRV8305_ISINK_50MA   = 0x3U,  // 50 mA
    DRV8305_ISINK_60MA   = 0x4U,  // 60 mA (Default)
    DRV8305_ISINK_70MA   = 0x5U,  // 70 mA
    DRV8305_ISINK_80MA   = 0x6U,  // 80 mA
    DRV8305_ISINK_250MA  = 0x7U,  // 0.25 A
    DRV8305_ISINK_500MA  = 0x8U,  // 0.50 A
    DRV8305_ISINK_750MA  = 0x9U,  // 0.75 A
    DRV8305_ISINK_1000MA = 0xAU,  // 1.00 A
    DRV8305_ISINK_1250MA = 0xBU   // 1.25 A
    // 0xC-0xF = 60 mA (per datasheet)
} drv8305_hs_isink_t;

/**
 * @brief High-side peak SOURCE current settings (IDRIVEP_HS)
 * Datasheet Reference: Table 14, Bits 3:0
 */
typedef enum 
{
    DRV8305_ISOURCE_10MA   = 0x0U,  // 10 mA
    DRV8305_ISOURCE_20MA   = 0x1U,  // 20 mA
    DRV8305_ISOURCE_30MA   = 0x2U,  // 30 mA
    DRV8305_ISOURCE_40MA   = 0x3U,  // 40 mA
    DRV8305_ISOURCE_50MA   = 0x4U,  // 50 mA (Default)
    DRV8305_ISOURCE_60MA   = 0x5U,  // 60 mA
    DRV8305_ISOURCE_70MA   = 0x6U,  // 70 mA
    DRV8305_ISOURCE_125MA  = 0x7U,  // 0.125 A
    DRV8305_ISOURCE_250MA  = 0x8U,  // 0.25 A
    DRV8305_ISOURCE_500MA  = 0x9U,  // 0.50 A
    DRV8305_ISOURCE_750MA  = 0xAU,  // 0.75 A
    DRV8305_ISOURCE_1000MA = 0xBU   // 1.00 A
    // 0xC-0xF = 50 mA (per datasheet)
} drv8305_hs_isource_t;

/**
 * @brief Low-side peak SINK current settings (IDRIVEN_LS)
 * Datasheet Reference: Table 15, Bits 7:4
 * Note: Same values as HS sink current
 */
typedef drv8305_hs_isink_t drv8305_ls_isink_t;

/**
 * @brief Low-side peak SOURCE current settings (IDRIVEP_LS)
 * Datasheet Reference: Table 15, Bits 3:0
 * Note: Same values as HS source current
 */
typedef drv8305_hs_isource_t drv8305_ls_isource_t;

/* -------------------------------------------------------------------------
 * Register 0x07: Gate Drive Control
 * Datasheet: Table 16, Page 41
 * ------------------------------------------------------------------------- */

/**
 * @brief Charge pump frequency selection
 * Datasheet Reference: Table 16, Bit 10
 */
typedef enum 
{
    DRV8305_VCPH_FREQ_518KHZ = 0x0U,  // Center = 518 kHz, Spread = 438-633 kHz (Default)
    DRV8305_VCPH_FREQ_452KHZ = 0x1U   // Center = 452 kHz, Spread = 419-491 kHz
} drv8305_vcph_freq_t;

/**
 * @brief Rectification control (1-PWM mode only)
 * Datasheet Reference: Table 16, Bit 9
 */
typedef enum 
{
    DRV8305_COMM_DIODE_FREEWHEEL  = 0x0U,  // Diode freewheeling
    DRV8305_COMM_ACTIVE_FREEWHEEL = 0x1U   // Active freewheeling (Default)
} drv8305_comm_option_t;

/**
 * @brief PWM input mode selection
 * Datasheet Reference: Table 16, Bits 8:7
 */
typedef enum 
{
    DRV8305_PWM_6_INPUTS = 0x0U,  // PWM with 6 independent inputs (Default)
    DRV8305_PWM_3_INPUTS = 0x1U,  // PWM with 3 independent inputs
    DRV8305_PWM_1_INPUT  = 0x2U   // PWM with 1 input
    // 0x3 = Reserved (treated as 6-input mode)
} drv8305_pwm_mode_t;

/**
 * @brief Dead time settings (added to minimum 280ns handshake time)
 * Datasheet Reference: Table 16, Bits 6:4
 */
typedef enum 
{
    DRV8305_DEADTIME_35NS   = 0x0U,  // 35 ns
    DRV8305_DEADTIME_52NS   = 0x1U,  // 52 ns (Default)
    DRV8305_DEADTIME_88NS   = 0x2U,  // 88 ns
    DRV8305_DEADTIME_440NS  = 0x3U,  // 440 ns
    DRV8305_DEADTIME_880NS  = 0x4U,  // 880 ns
    DRV8305_DEADTIME_1760NS = 0x5U,  // 1760 ns
    DRV8305_DEADTIME_3520NS = 0x6U,  // 3520 ns
    DRV8305_DEADTIME_5280NS = 0x7U   // 5280 ns
} drv8305_deadtime_t;

/**
 * @brief VDS sense blanking time (applies after gate turn-on)
 * Datasheet Reference: Table 16, Bits 3:2
 */
typedef enum 
{
    DRV8305_TBLANK_0US    = 0x0U,  // 0 µs
    DRV8305_TBLANK_1_75US = 0x1U,  // 1.75 µs (Default)
    DRV8305_TBLANK_3_5US  = 0x2U,  // 3.5 µs
    DRV8305_TBLANK_7US    = 0x3U   // 7 µs
} drv8305_tblank_t;

/**
 * @brief VDS sense deglitch time
 * Datasheet Reference: Table 16, Bits 1:0
 */
typedef enum 
{
    DRV8305_TVDS_0US    = 0x0U,  // 0 µs
    DRV8305_TVDS_1_75US = 0x1U,  // 1.75 µs
    DRV8305_TVDS_3_5US  = 0x2U,  // 3.5 µs (Default)
    DRV8305_TVDS_7US    = 0x3U   // 7 µs
} drv8305_tvds_t;

/* -------------------------------------------------------------------------
 * Register 0x09: IC Operation
 * Datasheet: Table 17, Page 42
 * ------------------------------------------------------------------------- */

/**
 * @brief Watchdog delay interval
 * Datasheet Reference: Table 17, Bits 6:5
 */
typedef enum 
{
    DRV8305_WD_DLY_10MS  = 0x0U,  // 10 ms
    DRV8305_WD_DLY_20MS  = 0x1U,  // 20 ms (Default)
    DRV8305_WD_DLY_50MS  = 0x2U,  // 50 ms
    DRV8305_WD_DLY_100MS = 0x3U   // 100 ms
} drv8305_wd_dly_t;

/* -------------------------------------------------------------------------
 * Register 0x0A: Shunt Amplifier Control
 * Datasheet: Table 18, Page 43
 * ------------------------------------------------------------------------- */

/**
 * @brief Current sense blanking time
 * Datasheet Reference: Table 18, Bits 7:6
 */
typedef enum 
{
    DRV8305_CS_BLANK_0NS   = 0x0U,  // 0 ns (Default)
    DRV8305_CS_BLANK_500NS = 0x1U,  // 500 ns
    DRV8305_CS_BLANK_2_5US = 0x2U,  // 2.5 µs
    DRV8305_CS_BLANK_10US  = 0x3U   // 10 µs
} drv8305_cs_blank_t;

/**
 * @brief Current sense amplifier gain settings
 * Datasheet Reference: Table 18, Bits 5:0 (per channel)
 */
typedef enum 
{
    DRV8305_GAIN_10V_V = 0x0U,  // 10 V/V (Default)
    DRV8305_GAIN_20V_V = 0x1U,  // 20 V/V
    DRV8305_GAIN_40V_V = 0x2U,  // 40 V/V
    DRV8305_GAIN_80V_V = 0x3U   // 80 V/V
} drv8305_gain_t;

/* -------------------------------------------------------------------------
 * Register 0x0B: Voltage Regulator Control
 * Datasheet: Table 19, Page 43
 * ------------------------------------------------------------------------- */

/**
 * @brief VREF scaling factor (k)
 * Datasheet Reference: Table 19, Bits 9:8
 * Note: VREF output = Internal_VREF / k
 */
typedef enum 
{
    DRV8305_VREF_SCALE_RSVD = 0x0U,  // Reserved
    DRV8305_VREF_SCALE_DIV2 = 0x1U,  // k = 2 (Default)
    DRV8305_VREF_SCALE_DIV4 = 0x2U,  // k = 4
    DRV8305_VREF_SCALE_DIV8 = 0x3U   // k = 8
} drv8305_vref_scale_t;

/**
 * @brief Sleep delay (VREG power-down delay after SLEEP command)
 * Datasheet Reference: Table 19, Bits 4:3
 */
typedef enum 
{
    DRV8305_SLEEP_DLY_0US   = 0x0U,  // 0 µs
    DRV8305_SLEEP_DLY_10US  = 0x1U,  // 10 µs (Default)
    DRV8305_SLEEP_DLY_50US  = 0x2U,  // 50 µs
    DRV8305_SLEEP_DLY_1MS   = 0x3U   // 1 ms
} drv8305_sleep_dly_t;

/**
 * @brief VREG undervoltage threshold
 * Datasheet Reference: Table 19, Bits 1:0
 */
typedef enum 
{
    DRV8305_VREG_UV_90PCT = 0x0U,  // VREG × 0.9
    DRV8305_VREG_UV_80PCT = 0x1U,  // VREG × 0.8
    DRV8305_VREG_UV_70PCT = 0x2U   // VREG × 0.7 (Default)
    // 0x3 = Same as 0x2
} drv8305_vreg_uv_level_t;

/* -------------------------------------------------------------------------
 * Register 0x0C: VDS Sense Control
 * Datasheet: Table 20, Page 44
 * ------------------------------------------------------------------------- */

/**
 * @brief VDS comparator threshold levels (full table)
 * Datasheet Reference: Table 20, Bits 7:3
 */
typedef enum
{
    DRV8305_VDS_0_060V = 0x00U, // 0.060 V
    DRV8305_VDS_0_068V = 0x01U, // 0.068 V
    DRV8305_VDS_0_076V = 0x02U, // 0.076 V
    DRV8305_VDS_0_086V = 0x03U, // 0.086 V

    DRV8305_VDS_0_097V = 0x04U, // 0.097 V
    DRV8305_VDS_0_109V = 0x05U, // 0.109 V
    DRV8305_VDS_0_123V = 0x06U, // 0.123 V
    DRV8305_VDS_0_138V = 0x07U, // 0.138 V

    DRV8305_VDS_0_155V = 0x08U, // 0.155 V
    DRV8305_VDS_0_175V = 0x09U, // 0.175 V
    DRV8305_VDS_0_197V = 0x0AU, // 0.197 V
    DRV8305_VDS_0_222V = 0x0BU, // 0.222 V

    DRV8305_VDS_0_250V = 0x0CU, // 0.250 V
    DRV8305_VDS_0_282V = 0x0DU, // 0.282 V
    DRV8305_VDS_0_317V = 0x0EU, // 0.317 V
    DRV8305_VDS_0_358V = 0x0FU, // 0.358 V

    DRV8305_VDS_0_403V = 0x10U, // 0.403 V
    DRV8305_VDS_0_454V = 0x11U, // 0.454 V
    DRV8305_VDS_0_511V = 0x12U, // 0.511 V
    DRV8305_VDS_0_576V = 0x13U, // 0.576 V

    DRV8305_VDS_0_648V = 0x14U, // 0.648 V
    DRV8305_VDS_0_730V = 0x15U, // 0.730 V
    DRV8305_VDS_0_822V = 0x16U, // 0.822 V
    DRV8305_VDS_0_926V = 0x17U, // 0.926 V

    DRV8305_VDS_1_043V = 0x18U, // 1.043 V
    DRV8305_VDS_1_175V = 0x19U, // 1.175 V (Default)
    DRV8305_VDS_1_324V = 0x1AU, // 1.324 V
    DRV8305_VDS_1_491V = 0x1BU, // 1.491 V

    DRV8305_VDS_1_679V = 0x1CU, // 1.679 V
    DRV8305_VDS_1_892V = 0x1DU, // 1.892 V
    DRV8305_VDS_2_131V = 0x1EU, // 2.131 V
    DRV8305_VDS_2_131V_DUP = 0x1FU // 2.131 V (same as 0x1E)
} drv8305_vds_level_t;


/**
 * @brief VDS overcurrent response mode
 * Datasheet Reference: Table 20, Bits 2:0
 */
typedef enum 
{
    DRV8305_VDS_MODE_LATCH_SHUTDOWN = 0x0U,  // Latched shutdown (Default)
    DRV8305_VDS_MODE_REPORT_ONLY    = 0x1U,  // Report only
    DRV8305_VDS_MODE_DISABLED       = 0x2U   // Disabled
    // 0x3-0x7 = Reserved
} drv8305_vds_mode_t;

/* ============================================================================
 * CONTROL REGISTER STRUCTURES & PACKING MACROS
 * ============================================================================ */

/**
 * @brief Register 0x05: HS Gate Drive Control
 * Datasheet Reference: Table 14, Page 40
 */
typedef struct 
{
    uint16_t tdrive;   // Bits 9:8 (drv8305_tdrive_t)
    uint16_t isink;    // Bits 7:4 (drv8305_hs_isink_t)
    uint16_t isource;  // Bits 3:0 (drv8305_hs_isource_t)
} drv8305_ctrl05_hs_gate_t;

#define DRV8305_PACK_CTRL05(cfg) \
    (uint16_t)(((cfg.tdrive  & 0x03U) << 8) | \
               ((cfg.isink   & 0x0FU) << 4) | \
               ((cfg.isource & 0x0FU) << 0))

/**
 * @brief Register 0x06: LS Gate Drive Control
 * Datasheet Reference: Table 15, Page 40
 */
typedef struct 
{
    uint16_t tdrive;   // Bits 9:8 (drv8305_tdrive_t)
    uint16_t isink;    // Bits 7:4 (drv8305_ls_isink_t)
    uint16_t isource;  // Bits 3:0 (drv8305_ls_isource_t)
} drv8305_ctrl06_ls_gate_t;

#define DRV8305_PACK_CTRL06(cfg) \
    (uint16_t)(((cfg.tdrive  & 0x03U) << 8) | \
               ((cfg.isink   & 0x0FU) << 4) | \
               ((cfg.isource & 0x0FU) << 0))

/**
 * @brief Register 0x07: Gate Drive Control
 * Datasheet Reference: Table 16, Page 41
 */
typedef struct 
{
    uint16_t vcph_freq;      // Bit 10 (drv8305_vcph_freq_t)
    uint16_t comm_option;    // Bit 9 (drv8305_comm_option_t)
    uint16_t pwm_mode;       // Bits 8:7 (drv8305_pwm_mode_t)
    uint16_t dead_time;      // Bits 6:4 (drv8305_deadtime_t)
    uint16_t tblank;         // Bits 3:2 (drv8305_tblank_t)
    uint16_t tvds;           // Bits 1:0 (drv8305_tvds_t)
} drv8305_ctrl07_gate_t;

#define DRV8305_PACK_CTRL07(cfg) \
    (uint16_t)(((cfg.vcph_freq   & 0x01U) << 10) | \
               ((cfg.comm_option & 0x01U) << 9)  | \
               ((cfg.pwm_mode    & 0x03U) << 7)  | \
               ((cfg.dead_time   & 0x07U) << 4)  | \
               ((cfg.tblank      & 0x03U) << 2)  | \
               ((cfg.tvds        & 0x03U) << 0))

/**
 * @brief Register 0x09: IC Operation
 * Datasheet Reference: Table 17, Page 42
 */
typedef struct 
{
    uint16_t flip_otsd;       // Bit 10 (1=Disable OTSD for DRV8305xE)
    uint16_t dis_pvdd_uvlo2;  // Bit 9
    uint16_t dis_gdrv_fault;  // Bit 8
    uint16_t en_sns_clamp;    // Bit 7
    uint16_t wd_dly;          // Bits 6:5 (drv8305_wd_dly_t)
    uint16_t dis_sns_ocp;     // Bit 4
    uint16_t wd_en;           // Bit 3
    uint16_t sleep;           // Bit 2
    uint16_t clr_flts;        // Bit 1 (Self-clearing)
    uint16_t set_vcph_uv;     // Bit 0
} drv8305_ctrl09_ic_op_t;

#define DRV8305_PACK_CTRL09(cfg) \
    (uint16_t)(((cfg.flip_otsd      & 0x01U) << 10) | \
               ((cfg.dis_pvdd_uvlo2 & 0x01U) << 9)  | \
               ((cfg.dis_gdrv_fault & 0x01U) << 8)  | \
               ((cfg.en_sns_clamp   & 0x01U) << 7)  | \
               ((cfg.wd_dly         & 0x03U) << 5)  | \
               ((cfg.dis_sns_ocp    & 0x01U) << 4)  | \
               ((cfg.wd_en          & 0x01U) << 3)  | \
               ((cfg.sleep          & 0x01U) << 2)  | \
               ((cfg.clr_flts       & 0x01U) << 1)  | \
               ((cfg.set_vcph_uv    & 0x01U) << 0))

/**
 * @brief Register 0x0A: Shunt Amplifier Control
 * Datasheet Reference: Table 18, Page 43
 */
typedef struct 
{
    uint16_t dc_cal_ch3;  // Bit 10 (DC calibration channel 3)
    uint16_t dc_cal_ch2;  // Bit 9 (DC calibration channel 2)
    uint16_t dc_cal_ch1;  // Bit 8 (DC calibration channel 1)
    uint16_t cs_blank;    // Bits 7:6 (drv8305_cs_blank_t)
    uint16_t gain_cs3;    // Bits 5:4 (drv8305_gain_t)
    uint16_t gain_cs2;    // Bits 3:2 (drv8305_gain_t)
    uint16_t gain_cs1;    // Bits 1:0 (drv8305_gain_t)
} drv8305_ctrl0A_shunt_t;

#define DRV8305_PACK_CTRL0A(cfg) \
    (uint16_t)(((cfg.dc_cal_ch3 & 0x01U) << 10) | \
               ((cfg.dc_cal_ch2 & 0x01U) << 9)  | \
               ((cfg.dc_cal_ch1 & 0x01U) << 8)  | \
               ((cfg.cs_blank   & 0x03U) << 6)  | \
               ((cfg.gain_cs3   & 0x03U) << 4)  | \
               ((cfg.gain_cs2   & 0x03U) << 2)  | \
               ((cfg.gain_cs1   & 0x03U) << 0))

/**
 * @brief Register 0x0B: Voltage Regulator Control
 * Datasheet Reference: Table 19, Page 43
 */
typedef struct
{
    uint16_t vref_scale;      // Bits 9:8 (drv8305_vref_scale_t)
    uint16_t sleep_dly;       // Bits 4:3 (drv8305_sleep_dly_t)
    uint16_t dis_vreg_pwrgd;  // Bit 2
    uint16_t vreg_uv_level;   // Bits 1:0 (drv8305_vreg_uv_level_t)
} drv8305_ctrl0B_vreg_t;

#define DRV8305_PACK_CTRL0B(cfg) \
    (uint16_t)(((cfg.vref_scale     & 0x03U) << 8) | \
               ((cfg.sleep_dly      & 0x03U) << 3) | \
               ((cfg.dis_vreg_pwrgd & 0x01U) << 2) | \
               ((cfg.vreg_uv_level  & 0x03U) << 0))

/**
 * @brief Register 0x0C: VDS Sense Control
 * Datasheet Reference: Table 20, Page 44
 */
typedef struct 
{
    uint16_t vds_level;  // Bits 7:3 (drv8305_vds_level_t)
    uint16_t vds_mode;   // Bits 2:0 (drv8305_vds_mode_t)
} drv8305_ctrl0C_vds_t;

#define DRV8305_PACK_CTRL0C(cfg) \
    (uint16_t)(((cfg.vds_level & 0x1FU) << 3) | \
               ((cfg.vds_mode  & 0x07U) << 0))


#ifdef __cplusplus
}
#endif

#endif /* DRV8305_CONTROL_REGISTERS_DEFINITIONS_H_ */
