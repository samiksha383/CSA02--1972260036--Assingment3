# Task 3: Final Design Recommendation & Architectural Defense

### Production System Recommendations
*   **Storage Data Mappings:** Use `unsigned short` for consumption tracking variables to save memory. Use `long long` for all monetary tracking fields to prevent calculations from overflowing at scale.
*   **Fixed Base Constants:** Use preprocessor `#define` directives for compile-time values to optimize build-level configurations.
*   **Input Channel Validations:** Use signed integers (`int`) when reading raw values from inputs. This allows the system to catch and reject negative values before they enter the processing pipeline.

### Tariff Revision Workflow
When updating utility rates using a compile-time configuration:
1. Update the values in the `billing.h` header file:
   ```c
   #define SLAB2_RATE_PAISE 250 // Raised to Rs 2.50
   ```
2. Recompile the source code using the system build tool: `gcc billing.c -o billing_system`.
3. Distribute the updated binary across the billing infrastructure. This approach ensures the new rates are applied consistently across all calculations.

### System Scale Limitations
*   The system can process up to 4,294,967,295 consumer records when using an `unsigned int` for primary keys.
*   Individual household meters are assumed to follow standard 16-bit limits (0–65535). If a household switches to a larger 32-bit meter, consumption values will be miscalculated as errors unless the validation thresholds are updated.

### Alternative Concept & Complexity Analysis
Transitioning the system to read tariff rules from an external configuration file at runtime instead of using hardcoded variables:
*   **Startup Overhead:** Reading a text file at startup introduces an $O(t)$ time complexity overhead, where $t$ is the number of configuration lines. For small text files, this step takes only a few milliseconds, which is negligible for a monthly batch billing application.
*   **Memory Footprint:** Requires a small amount of memory ($O(1)$ space complexity) to store the configuration values in a structural struct during execution.
*   **Operational Trade-offs:** An external configuration file allows administrators to adjust billing rates instantly without recompiling the program. However, this approach requires adding validation logic to ensure the configuration file cannot be tampered with or corrupted, which could cause system-wide calculation errors.
