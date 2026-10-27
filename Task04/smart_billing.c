#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int active_slab_limits[2];
    int base_slab_rates[3];
    int fixed_base_fee;
} RuntimeTariffConfig;

/* Evaluates a single 15-minute smart meter reading log entry */
long long process_interval_charge(int watt_hours, int hour_24, RuntimeTariffConfig *config) {
    /* Convert input watt-hours to standard kilowatt-hours (kWh) units */
    double kwh_consumed = (double)watt_hours / 1000.0;
    
    /* Determine initial base rate per unit based on current slab logic */
    /* For high-frequency processing, rates apply directly to the interval consumption */
    double applicable_base_rate = config->base_slab_rates[0]; 
    
    /* Check if usage falls between 6 PM (18:00) and 10 PM (22:00) to apply the 20% Peak premium */
    if (hour_24 >= 18 && hour_24 < 22) {
        applicable_base_rate = applicable_base_rate * 1.20; /* Apply peak surcharge */
    }

    /* Compute total cost in integer paise, rounding correctly to prevent precision drift */
    return (long long)((kwh_consumed * applicable_base_rate) + 0.5);
}

int main(void) {
    RuntimeTariffConfig active_config = {{100, 200}, {0, 225, 450}, 5000};
    
    printf("--- High-Frequency Smart Grid Billing Simulation ---\n");
    
    /* Example: 1200 Watt-hours used during an Off-Peak interval (2 PM) */
    long long off_peak_charge = process_interval_charge(1200, 14, &active_config);
    
    /* Example: 1200 Watt-hours used during a Peak interval (7 PM) */
    long long peak_charge = process_interval_charge(1200, 19, &active_config);

    printf("Off-Peak Interval Cost (14:00): %lld paise\n", off_peak_charge);
    printf("Peak Interval Cost (19:00)    : %lld paise (Includes 20%% Surcharge)\n", peak_charge);

    return 0;
