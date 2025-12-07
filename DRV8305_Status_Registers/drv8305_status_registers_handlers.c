/**
 * @file drv8305_status_registers_handlers.c
 * @brief DRV8305 Status Register Handlers - Implementation
 * @details Implements handler functions for status register reads with fault/warning
 *          flag checking and application-level error handling logic.
 * @author Furkan YAYLA
 * @email yaylafurkan@protonmail.com
 * @date December 2025
 * @version 1.0
 * 
 * @purpose
 * This module provides status register handlers called during register polling:
 *   - Register 0x01: Warning handler (temperature thresholds, supply faults)
 *   - Register 0x02: OV/VDS handler (overcurrent protection, sense faults)
 *   - Register 0x03: IC Faults handler (internal IC health, shutdown events)
 *   - Register 0x04: VGS Faults handler (gate drive circuit failures)
 * 
 * @fault_response_strategy
 * - Temperature warnings: Throttle motor performance
 * - Supply faults (PVDD, VCPH): Stop motor operation
 * - VDS/VGS faults: Immediate protective shutdown
 * - Watchdog faults: System diagnostics/logging
 * 
 * @datasheet_reference
 * DRV8305-Q1 Status Registers (Pages 38-39, Table 10-13)
 * Temperature Thresholds (Page 38):
 *   FLAG1: ~105°C, FLAG2: ~125°C, FLAG3: ~135°C, FLAG4/OTW: ~150°C
 * Fault Priority: VDS/VGS > IC Faults > OV/VDS Faults > Warnings
 */

#include <stdint.h>

#include "drv8305_macros.h"
#include "drv8305_status_registers_definitions.h"
#include "drv8305_status_registers_handlers.h"

/* --------------------------- STATUS REGISTERS HANDLERS --------------------------- */

/**
 * @brief Handle DRV8305 Status Register 0x01 (Warning & Watchdog Reset)
 * @details Processes warning flags including overtemperature, charge pump undervoltage,
 *          PVDD faults, and VDS monitoring status. Triggered during status polling cycle.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Status register 0x01 data bits
 * @return None
 * @note This is a callback function - implement application-specific warning handling
 * @see DRV8305_WARN_OTW, DRV8305_WARN_VCPH_UVFL, DRV8305_WARN_PVDD_OVFL
 */
DRV8305_PUBLIC void drv8305_warning_register_handler(void *self, uint16_t data)
{
    // Register 0x01: Warning & Watchdog Reset
    (void)self;  // Unused parameter
    
    // Overtemperature warning (~150°C)
    if (data & DRV8305_WARN_OTW) 
    {
        // Action: Decrease motor performance
    }

    // Temperature flag 3 (~135°C)
    if (data & DRV8305_WARN_TEMP_FLAG3) 
    {
        
    }

    // Temperature flag 2 (~125°C)
    if (data & DRV8305_WARN_TEMP_FLAG2) 
    {
        
    }

    // Temperature flag 1 (~105°C)
    if (data & DRV8305_WARN_TEMP_FLAG1) 
    {
    
    }

    // Charge pump undervoltage flag warning
    if (data & DRV8305_WARN_VCPH_UVFL) 
    {
    
    }

    // VDS overcurrent monitor (OR of all VDS monitors)
    if (data & DRV8305_WARN_VDS_STATUS) 
    {
    
    }

    // PVDD overvoltage flag warning
    if (data & DRV8305_WARN_PVDD_OVFL) 
    {
        
    }

    // PVDD undervoltage flag warning
    if (data & DRV8305_WARN_PVDD_UVFL) 
    {
    
    }

    // Temperature flag 4 (~175°C)
    if (data & DRV8305_WARN_TEMP_FLAG4) 
    {
    
    }
    
    // Global fault indication
    if (data & DRV8305_WARN_FAULT) 
    {
        // Action: This bit being set indicates an IC Fault or VGS Fault
    }
}

/**
 * @brief Handle DRV8305 Status Register 0x02 (OV/VDS Faults)
 * @details Processes overcurrent faults on shunt current sense channels and VDS
 *          overcurrent faults for all six MOSFETs (high-side and low-side).
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Status register 0x02 data bits
 * @return None
 * @note Fault bits indicate hardware overcurrent conditions - implement protective shutdown
 * @see DRV8305_VDS_SNS_A_OCP, DRV8305_VDS_HA, DRV8305_VDS_LA
 */
DRV8305_PUBLIC void drv8305_ov_vds_register_handler(void *self, uint16_t data)
{
    // Register 0x02: OV/VDS Faults
    (void)self;  // Unused parameter
    
    // Sense A overcurrent fault
    if (data & DRV8305_VDS_SNS_A_OCP) 
    {
        // Action: Immediately stop the motor, check the FAULT pin
    }

    // Sense B overcurrent fault
    if (data & DRV8305_VDS_SNS_B_OCP) 
    {
    
    }

    // Sense C overcurrent fault
    if (data & DRV8305_VDS_SNS_C_OCP) 
    {
    
    }
    
    // VDS overcurrent fault for low-side MOSFET C
    if (data & DRV8305_VDS_LC) 
    {
    
    }

    // VDS overcurrent fault for high-side MOSFET C
    if (data & DRV8305_VDS_HC) 
    {
    
    }

    // VDS overcurrent fault for low-side MOSFET B
    if (data & DRV8305_VDS_LB) 
    {
    
    }

    // VDS overcurrent fault for high-side MOSFET B
    if (data & DRV8305_VDS_HB) 
    {
    
    }

    // VDS overcurrent fault for low-side MOSFET A
    if (data & DRV8305_VDS_LA) 
    {
    
    }

    // VDS overcurrent fault for high-side MOSFET A
    if (data & DRV8305_VDS_HA) 
    {
    
    }
}

/**
 * @brief Handle DRV8305 Status Register 0x03 (IC Faults)
 * @details Processes internal IC faults including charge pump faults, power supply
 *          undervoltage (PVDD, AVDD, VREG), overtemperature shutdown, and watchdog timer.
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Status register 0x03 data bits
 * @return None
 * @note These are internal IC health indicators - implement diagnostic and recovery logic
 * @see DRV8305_IC_VCPH_OVLO_ABS, DRV8305_IC_OTSD, DRV8305_IC_WD_FAULT
 */
DRV8305_PUBLIC void drv8305_ic_faults_register_handler(void *self, uint16_t data)
{
    // Register 0x03: IC Faults
    (void)self;  // Unused parameter
    
    // Charge pump absolute overvoltage fault
    if (data & DRV8305_IC_VCPH_OVLO_ABS) 
    {
        // Action: Stop the motor. These bits indicate the internal health of the DRV8305.
    }

    // Charge pump overvoltage fault (relative to PVDD)
    if (data & DRV8305_IC_VCPH_OVLO) 
    {
    
    }

    // Charge pump undervoltage 2 fault
    if (data & DRV8305_IC_VCPH_UVLO2) 
    {
    
    }
    
    // Low-side gate supply undervoltage fault
    if (data & DRV8305_IC_VCP_LSD_UVLO2) 
    {
    
    }

    // AVDD undervoltage fault
    if (data & DRV8305_IC_AVDD_UVLO) 
    {
    
    }

    // VREG undervoltage fault
    if (data & DRV8305_IC_VREG_UV) 
    {
    
    }

    // Overtemperature shutdown fault (~175°C)
    if (data & DRV8305_IC_OTSD) 
    {
    
    }

    // Watchdog fault
    if (data & DRV8305_IC_WD_FAULT) 
    {
    
    }

    // PVDD undervoltage 2 fault
    if (data & DRV8305_IC_PVDD_UVLO2) 
    {
    
    }
}

/**
 * @brief Handle DRV8305 Status Register 0x04 (VGS Faults)
 * @details Processes gate driver faults for all six MOSFETs indicating gate drive
 *          circuitry failures on high-side and low-side switches (A, B, C phases).
 * @param[in] self Pointer to DRV8305 user object context
 * @param[in] data Status register 0x04 data bits
 * @return None
 * @note Gate drive faults indicate MOSFET drive circuit issues - stop motor immediately
 * @see DRV8305_VGS_HA, DRV8305_VGS_HB, DRV8305_VGS_HC, DRV8305_VGS_LA
 */
DRV8305_PUBLIC void drv8305_vgs_faults_register_handler(void *self, uint16_t data)
{
    // Register 0x04: VGS Faults
    (void)self;  // Unused parameter
    
    // VGS gate drive fault for low-side MOSFET C
    if (data & DRV8305_VGS_LC) 
    {
        // Action: Immediately stop the motor, log that the relevant MOSFET is faulty.        
    }

    // VGS gate drive fault for high-side MOSFET C
    if (data & DRV8305_VGS_HC) 
    {
    
    }

    // VGS gate drive fault for low-side MOSFET B
    if (data & DRV8305_VGS_LB) 
    {
    
    }

    // VGS gate drive fault for high-side MOSFET B
    if (data & DRV8305_VGS_HB) 
    {
    
    }

    // VGS gate drive fault for low-side MOSFET A
    if (data & DRV8305_VGS_LA) 
    {
    
    }

    // VGS gate drive fault for high-side MOSFET A
    if (data & DRV8305_VGS_HA) 
    {
        
    }
}