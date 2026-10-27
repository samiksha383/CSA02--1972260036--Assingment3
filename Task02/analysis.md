# Task 2: Data Scale Precision & Overflow Analysis

### Precision Drift Comparison Matrix
When simulating identical records ($250\text{ units} = \text{Rs } 500.00$) over scaling regions, precision drift becomes visible:

*   **Village Scale ($4,000$ households):** Floating-point and integer balances match closely because the value fits within standard precision margins.
*   **Taluk Scale ($60,000$ households):** Small rounding errors begin to appear in the floating-point calculations.
*   **District Scale ($800,000$ households):** The accumulated floating-point error becomes noticeable, drifting away from the exact integer total.
*   **Root Cause:** Floating-point types use binary fractions ($\frac{1}{2}, \frac{1}{4}$), which cannot exactly represent base-10 decimals like `0.25`. Adding these values repeatedly causes the rounding errors to accumulate over time.

### Scalability Overflow Threshold Analysis
At a district scale ($800,000$ homes) with an average bill of Rs 500.00 ($50,000\text{ paise}$), the total collection is $40,000,000,000\text{ paise}$ ($4 \times 10^{10}$).

*   **`int` (Minimum 16-bit, typically 32-bit on current systems):** Max limit for a signed 32-bit int is $+2,147,483,647$. **A district-level total will overflow a standard 32-bit integer.**
*   **`long` (Minimum 32-bit, platform dependent):** On 32-bit systems, `long` is identical to `int` and overflows. On 64-bit platforms, it handles up to $\approx 9.22 \times 10^{18}$ safely.
*   **`long long` (Guaranteed minimum 64-bit):** Caps at $+9,223,372,036,854,775,807$. It can easily handle the district total ($4 \times 10^{10}\text{ paise}$) with room to scale further.

### Hardware vs. Language Specification Audit

| Variable Type | Minimum Size (C Standard) | Actual Size (x86\_64 GCC) | Technical Implications / Architecture Impact |
| :--- | :--- | :--- | :--- |
| `int` | 16 Bits | 32 Bits (4 Bytes) | Provides larger ranges locally, but can hide overflow bugs if written for 16-bit systems. |
| `long` | 32 Bits | 64 Bits (8 Bytes) | Varies by operating system (LLP64 on Windows vs LP64 on Linux), which can cause cross-platform issues. |
| `long long` | 64 Bits | 64 Bits (8 Bytes) | Consistently large across all modern compilers, making it the safest choice for financial calculations. |

### Alternative Concept & Complexity Analysis
Accumulating the district financial totals in a 64-bit `double` precision float instead of a `long long` integer:
*   **Overflow vs. Precision Loss:** A `double` has a large range ($\approx 1.7 \times 10^{308}$) and will not overflow. However, it only provides 53 bits of precision ($\approx 15\text{–}17$ decimal digits). When adding small values to a very large accumulated total, the lower bits of the small numbers are truncated, leading to precision loss. A `long long` preserves exact accuracy to the single paisa across its entire range.
*   **Computational Overhead:** Modern x86 processors can perform integer addition and floating-point addition at comparable speeds, but integer operations remain slightly more efficient because they do not require normalization steps. Both types occupy exactly 8 bytes ($O(1)$ space complexity).
