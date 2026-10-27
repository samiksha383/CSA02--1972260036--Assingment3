#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int limit1;
    int limit2;
    int rate1;
    int rate2;
    int rate3;
    int fixed_charge;
} DynamicTariff;

/* Simulation of an external configuration load */
int load_tariff_configuration(DynamicTariff *tariff) {
    /* Simulating reading values from a structural runtime config text file */
    tariff->limit1 = 100;
    tariff->limit2 = 200;
    tariff->rate1 = 0;      /* Free */
    tariff->rate2 = 225;    /* Rs 2.25 */
    tariff->rate3 = 450;    /* Rs 4.50 */
    tariff->fixed_charge = 5000;
    return 0;
}

int main(void) {
    DynamicTariff current_tariff;
    
    if (load_tariff_configuration(&current_tariff) != 0) {
        fprintf(stderr, "Failed to inject operational configuration parameters.\n");
        return 1;
    }

    printf("--- Configuration Driven Engine Status ---\n");
    printf("Tariff Parameters Parsed Successfully:\n");
    printf("  Fixed Base Cost  : %d paise\n", current_tariff.fixed_charge);
    printf("  Slab Tier 3 Rate : %d paise/unit\n", current_tariff.rate3);
    
    return 0;
}
