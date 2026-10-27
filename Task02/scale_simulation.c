#include <stdio.h>

void execute_scale_simulation(long long total_bills) {
    double float_version_total = 0.0;
    long long paise_version_total = 0;

    /* Simulate a consistent billing profile across various scales */
    /* Average bill: 250 units -> Rs 50.00 + (100*2.25) + (50*4.50) = Rs 500.00 -> 50000 paise */
    long long single_bill_paise = 50000;
    double single_bill_float = 500.00;

    for (long long i = 0; i < total_bills; i++) {
        paise_version_total += single_bill_paise;
        float_version_total += single_bill_float;
    }

    double conversion_difference = (double)paise_version_total / 100.0 - float_version_total;

    printf("Scale: %lld Households\n", total_bills);
    printf("  Integer Paise Conversion Total : Rs %.2f\n", (double)paise_version_total / 100.0);
    printf("  Standard Float Accumulated Total: Rs %.2f\n", float_version_total);
    printf("  Calculated Accumulated Drift   : Rs %.6f\n\n", conversion_difference);
}

int main(void) {
    printf("--- Platform Limits Reference Matrix ---\n");
    printf("sizeof(int)       = %zu bytes\n", sizeof(int));
    printf("sizeof(long)      = %zu bytes\n", sizeof(long));
    printf("sizeof(long long) = %zu bytes\n\n", sizeof(long long));

    execute_scale_simulation(4000);   /* Village scale */
    execute_scale_simulation(60000);  /* Taluk scale */
    execute_scale_simulation(800000); /* District scale */

    return 0;
}
