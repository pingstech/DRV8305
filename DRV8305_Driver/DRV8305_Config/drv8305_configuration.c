/**
 * @file drv8305_configuration.c
 * @brief DRV8305 Configuration Module - Data Definitions
 * @details Contains default configuration instance with DRV8305-Q1 datasheet values.
 *          Function implementations (drv8305_get_configuration, drv8305_set_configuration)
 *          have been moved to drv8305_api.c for unified API implementation.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This implementation file:
 *   - Defines default_configuration instance with DRV8305-Q1 datasheet reset values
 *   - Note: getter/setter function implementations are in drv8305_api.c
 * 
 * @datasheet_reference
 * DRV8305-Q1 Functional Block Diagram (Page 36)
 * Register Default Values (Pages 40-44, "Reset Value" columns)
 * 
 * @memory_layout
 * Configuration structure contains 7 nested typedef structures:
 *   Index 0: drv8305_ctrl05_hs_gate_t   (Register 0x05: HS Gate Drive Control)
 *   Index 1: drv8305_ctrl06_ls_gate_t   (Register 0x06: LS Gate Drive Control)
 *   Index 2: drv8305_ctrl07_gate_t      (Register 0x07: Gate Drive Control)
 *   Index 3: drv8305_ctrl09_ic_op_t     (Register 0x09: IC Operation)
 *   Index 4: drv8305_ctrl0A_shunt_t     (Register 0x0A: Shunt Amplifier Control)
 *   Index 5: drv8305_ctrl0B_vreg_t      (Register 0x0B: Voltage Regulator Control)
 *   Index 6: drv8305_ctrl0C_vds_t       (Register 0x0C: VDS Sense Control)
 */

#include <string.h>

#include "drv8305_macros.h"
#include "drv8305_configuration.h"

/* ---------------------------------------------------------------------------
 * DRV8305 Default Configuration Object
 * Reference: DRV8305-Q1 Datasheet (Pages 40-44, "Reset Value" columns)
 * --------------------------------------------------------------------------- */
DRV8305_PUBLIC drv8305_configuration_t default_configuration =
{
    /* -----------------------------------------------------------------------
     * Register 0x05: HS Gate Drive Control
     * Default Value: 0x344
     * ----------------------------------------------------------------------- */
    .hs_gate_drive = 
    {
        .tdrive  = DRV8305_TDRIVE_1780NS,    // Bits 9:8  - Default: 1780 ns
        .isink   = DRV8305_HS_ISINK_60MA,    // Bits 7:4  - Default: 60 mA
        .isource = DRV8305_HS_ISOURCE_50MA   // Bits 3:0  - Default: 50 mA
    },

    /* -----------------------------------------------------------------------
     * Register 0x06: LS Gate Drive Control
     * Default Value: 0x344
     * Note: Uses HS typedefs for sink/source as per handlers.h
     * ----------------------------------------------------------------------- */
    .ls_gate_drive = 
    {
        .tdrive  = DRV8305_TDRIVE_1780NS,    // Bits 9:8  - Default: 1780 ns
        .isink   = DRV8305_HS_ISINK_60MA,    // Bits 7:4  - Default: 60 mA
        .isource = DRV8305_HS_ISOURCE_50MA   // Bits 3:0  - Default: 50 mA
    },

    /* -----------------------------------------------------------------------
     * Register 0x07: Gate Drive Control
     * Default Value: 0x296
     * ----------------------------------------------------------------------- */
    .gate_drive = 
    {
        .vcph_freq   = DRV8305_VCPH_FREQ_518KHZ,      // Bit 10   - Default: 518 kHz
        .comm_option = DRV8305_COMM_ACTIVE_FREEWHEEL, // Bit 9    - Default: Active Freewheeling
        .pwm_mode    = DRV8305_PWM_6_INPUTS,          // Bits 8:7 - Default: 6 Inputs
        .dead_time   = DRV8305_DEADTIME_52NS,         // Bits 6:4 - Default: 52 ns
        .tblank      = DRV8305_TBLANK_1_75US,         // Bits 3:2 - Default: 1.75 us
        .tvds        = DRV8305_TVDS_3_5US             // Bits 1:0 - Default: 3.5 us
    },

    /* -----------------------------------------------------------------------
     * Register 0x09: IC Operation
     * Default Value: Depends on variant (0x400 typically)
     * ----------------------------------------------------------------------- */
    .ic_operation = 
    {
        .flip_otsd      = 0,                    // Bit 10   - Default: Enable OTSD (0) for Q variant
        .dis_pvdd_uvlo2 = 0,                    // Bit 9    - Default: Enabled (0)
        .dis_gdrv_fault = 0,                    // Bit 8    - Default: Enabled (0)
        .en_sns_clamp   = 0,                    // Bit 7    - Default: Disabled (0)
        .wd_dly         = DRV8305_WD_DLY_20MS,  // Bits 6:5 - Default: 20 ms
        .dis_sns_ocp    = 0,                    // Bit 4    - Default: Enabled (0)
        .wd_en          = 0,                    // Bit 3    - Default: Disabled (0)
        .sleep          = 0,                    // Bit 2    - Default: Awake (0)
        .clr_flts       = 1,                    // Bit 1    - Default: Normal Operation (0) | Clear Faults (1)
        .set_vcph_uv    = 0                     // Bit 0    - Default: 4.9V (0)
    },

    /* -----------------------------------------------------------------------
     * Register 0x0A: Shunt Amplifier Control
     * Default Value: 0x000
     * ----------------------------------------------------------------------- */
    .shunt_amplifier = 
    {
        .dc_cal_ch3 = 0,                        // Bit 10   - Default: Normal Operation (0)
        .dc_cal_ch2 = 0,                        // Bit 9    - Default: Normal Operation (0)
        .dc_cal_ch1 = 0,                        // Bit 8    - Default: Normal Operation (0)
        .cs_blank   = DRV8305_CS_BLANK_0NS,     // Bits 7:6 - Default: 0 ns
        .gain_cs3   = DRV8305_GAIN_10V_V,       // Bits 5:4 - Default: 10 V/V
        .gain_cs2   = DRV8305_GAIN_10V_V,       // Bits 3:2 - Default: 10 V/V
        .gain_cs1   = DRV8305_GAIN_10V_V        // Bits 1:0 - Default: 10 V/V
    },

    /* -----------------------------------------------------------------------
     * Register 0x0B: Voltage Regulator Control
     * Default Value: 0x102
     * ----------------------------------------------------------------------- */
    .voltage_regulator = 
    {
        .vref_scale     = DRV8305_VREF_SCALE_DIV2,    // Bits 9:8 - Default: k = 2
        .sleep_dly      = DRV8305_SLEEP_DLY_10US,     // Bits 4:3 - Default: 10 us
        .dis_vreg_pwrgd = 0,                          // Bit 2    - Default: Enabled (0)
        .vreg_uv_level  = DRV8305_VREG_UV_70PCT       // Bits 1:0 - Default: VREG x 0.7
    },

    /* -----------------------------------------------------------------------
     * Register 0x0C: VDS Sense Control
     * Default Value: 0x0CA
     * ----------------------------------------------------------------------- */
    .vds_sense = 
    {
        .vds_level = DRV8305_VDS_1_175V,              // Bits 7:3 - Default: 1.175 V
        .vds_mode  = DRV8305_VDS_MODE_LATCH_SHUTDOWN  // Bits 2:0 - Default: Latched Shutdown
    }
};
