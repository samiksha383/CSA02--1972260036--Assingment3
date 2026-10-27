#include "billing.h"

int validate_and_calculate_units(int prev, int curr, unsigned int *units) {
    /* Rule 1: Reject explicit negative inputs */
    if (prev < 0 || curr < 0) return -1;
    
    /* Rule 2: Reject inputs exceeding 16-bit hardware limits */
    if (prev > MAX_METER_VALUE || curr > MAX_METER_VALUE) return -2;

    /* Rule 3: Process normal or hardware meter rollover cases */
    if (curr >= prev) {
        *units = (unsigned int)(curr - prev);
    } else {
        /* Check if it is a legitimate hardware rollover or just an invalid decrease */
        /* If difference is unreasonably large downward without crossing 0, it might be human error, */
        /* but standard hardware rollover formula applies on 16-bit boundaries: */
        *units = (unsigned int)((MAX_METER_VALUE - prev) + curr + 1);
    }
    return 0; /* Validated successfully */
}

long long calculate_energy_charge(unsigned int units) {
    long long charge = 0;
    if (units <= SLAB1_LIMIT) {
        charge = (long long)units * SLAB1_RATE_PAISE;
    } else if (units <= SLAB2_LIMIT) {
        charge = ((long long)SLAB1_LIMIT * SLAB1_RATE_PAISE) + 
                 ((long long)(units - SLAB1_LIMIT) * SLAB2_RATE_PAISE);
    } else {
        charge = ((long long)SLAB1_LIMIT * SLAB1_RATE_PAISE) + 
                 ((long long)(SLAB2_LIMIT - SLAB1_LIMIT) * SLAB2_RATE_PAISE) + 
                 ((long long)(units - SLAB2_LIMIT) * SLAB3_RATE_PAISE);
    }
    return charge;
}

int main(void) {
    BillRecord record;
    int scan_status;
    long long cumulative_total_paise = 0;
    int record_count = 0;

    printf("%-10s %-10s %-10s %-10s %-15s\n", "CustID", "Prev", "Curr", "Units", "Total Bill");
    printf("------------------------------------------------------------\n");

    while ((scan_status = scanf("%u %d %d", &record.consumer_id, &record.prev_reading, &record.curr_reading)) != EOF) {
        if (scan_status != 3) {
            printf("[ERROR] Malformed input structure detected. Skipping line.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        int status = validate_and_calculate_units(record.prev_reading, record.curr_reading, &record.units_consumed);
        
        if (status < 0) {
            printf("%-10u %-10d %-10d %-10s [INVALID INPUT]\n", 
                   record.consumer_id, record.prev_reading, record.curr_reading, "N/A");
            continue;
        }

        record.energy_charge_paise = calculate_energy_charge(record.units_consumed);
        record.total_bill_paise = record.energy_charge_paise + FIXED_CHARGE_PAISE;
        
        cumulative_total_paise += record.total_bill_paise;
        record_count++;

        long long rupees = record.total_bill_paise / 100;
        long long paise = record.total_bill_paise % 100;

        printf("%-10u %-10d %-10d %-10u Rs %lld.%02lld\n",
               record.consumer_id, record.prev_reading, record.curr_reading,
               record.units_consumed, rupees, paise);
    }

    printf("------------------------------------------------------------\n");
    printf("Processed %d valid records. Grand Total: Rs %lld.%02lld\n", 
           record_count, cumulative_total_paise / 100, cumulative_total_paise % 100);

    return 0;
}
