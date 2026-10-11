# Relief Supply Distribution System

**CS-110 Fundamentals of Computer Programming - Assignment 1**
Class: BESE-17 A, SEECS, NUST
Submitted to: Dr. Momina Moetesum

A simple C++ console program for a district office after a flood. The office has a limited number of trucks and supply packs. For each relief request, the program calculates a need score, chooses an action, and updates the stock.

## Repository Contents

| File | Description |
|---|---|
| `relief_supply.cpp` | C++ source code |
| `relief_supply_report.docx` | C++ report file |
| `flowchart.png` | Flowchart of the program |
| `README.md` | This file |

## Group Members

| Name | Student ID | Main work |
|---|---|---|
| Syed Muhammad Aliyan Ali | 585981 | Need score formula and reasons |
| Affan Akbar | 592135 | Decision rules, stock check and fallback, flowchart |
| Muhammad Yahya Shahzad | 604750 | Test sets, README, Git commits |

## How to Compile and Run

Linux / macOS:
```bash
g++ -std=c++11 -o relief_supply relief_supply.cpp
./relief_supply
```

Windows (Command Prompt or PowerShell):
```bash
g++ relief_supply.cpp -o relief_supply.exe
relief_supply.exe
```

## Program Input

The program first asks for the starting stock, then asks 6 questions for each of the 5 requests.

| Input | What to type |
|---|---|
| Starting trucks | A whole number, for example `10` |
| Starting supply packs | A whole number, for example `300` |
| Damage level | `1` = Low, `2` = Medium, `3` = High |
| Families affected | A number from `10` to `200` |
| Access route | `1` = Open, `2` = Damaged |
| Flood warning | `y` = Yes, `n` = No |
| Medical need | `y` = Yes, `n` = No |
| Previous aid received | `y` = Yes, `n` = No |

Wrong values (for example letters where a number is needed) are rejected and the question is asked again. Capital `Y` and `N` are also accepted.

## Need Score

Formula:

```
Need Score = Damage points + Family points + Route points
           + Flood points + Medical points - Previous aid penalty
```

| Input | Points | Why this weight |
|---|---|---|
| Damage level | Low = 5, Medium = 15, High = 25 | Shows how much of the area is destroyed. High damage is the most urgent. |
| Families affected | Families / 10 (10 families = 1 point, 200 = 20) | More families need more food. Kept small so that a big area with little damage does not always win. |
| Access route | Damaged = +15, Open = 0 | A damaged road slows or stops trucks, so help must be planned earlier. |
| Flood warning | Yes = +10, No = 0 | More flooding may come soon. Early help can stop things getting worse. |
| Medical need | Yes = +30, No = 0 | People who need medicine cannot wait. This is the largest single weight. |
| Previous aid received | Yes = -10, No = 0 | Areas that already got help come after areas with no help. |

Range: the lowest possible score is **-4**, the highest is **100**.

Example, Riverside Village (High damage, 150 families, damaged route, flood warning, medical need, no previous aid):
`25 + 15 + 15 + 10 + 30 + 0 = 95`

## Decision Rules

**1. Wanted action from the score**

| Need score | Wanted action | Trucks needed | Supply packs needed |
|---|---|---|---|
| 70 or more | A - Full Relief | 2 | 50 |
| 40 to 69 | B - Standard Relief | 1 | 30 |
| 20 to 39 | C - Emergency Kit Only | 1 | 10 |
| Below 20 | D - Defer / Monitor | 0 | 0 |

**2. Stock check and fallback**

1. Find the wanted action from the table above.
2. Check that trucks left >= trucks needed **and** packs left >= packs needed.
3. If yes, use this action.
4. If no, try the next smaller action: A -> B -> C -> D.
5. If nothing fits, the request is not served. Action D always fits because it needs nothing.
6. Subtract the used trucks and packs. The stock is checked before subtracting, so it never goes below zero.

**Why these rules**
- The limits 70, 40 and 20 split the score into four clear bands, with one action for each.
- Fallback is better than nothing. If only 1 truck is left, Standard Relief is sent instead of Full Relief, which needs 2 trucks.
- Low scores are deferred so that stock is kept for areas in urgent need. The request is marked for monitoring.
- A medical request always gets at least 26 points (the lowest possible), so it is never deferred. This is why no separate medical rule is needed.

## Flowchart

See `flowchart.png`. It follows the same steps as the code: read stock, repeat for 5 requests (read inputs, score, choose wanted action, check stock with fallback, subtract, print), then print the summary.

## Test Cases

All test sets use the same five requests:

| Request | Damage | Families | Route | Flood | Medical | Previous aid |
|---|---|---|---|---|---|---|
| R1 Riverside Village | 3 (High) | 150 | 2 (Damaged) | y | y | n |
| R2 Hilltop Settlement | 2 (Medium) | 60 | 1 (Open) | n | n | y |
| R3 Lakeside Camp | 3 (High) | 90 | 1 (Open) | y | y | n |
| R4 Market Town | 1 (Low) | 200 | 1 (Open) | n | n | n |
| R5 Bridge Colony | 3 (High) | 40 | 2 (Damaged) | y | y | y |

### Test Set 1: Sufficient resources (start: 10 trucks, 300 packs)

| Request | Score | Wanted | Action chosen | Trucks used | Packs used | Trucks left | Packs left |
|---|---|---|---|---|---|---|---|
| R1 | 95 | A | A - Full Relief | 2 | 50 | 8 | 250 |
| R2 | 11 | D | D - Defer / Monitor | 0 | 0 | 8 | 250 |
| R3 | 74 | A | A - Full Relief | 2 | 50 | 6 | 200 |
| R4 | 25 | C | C - Emergency Kit Only | 1 | 10 | 5 | 190 |
| R5 | 74 | A | A - Full Relief | 2 | 50 | 3 | 140 |

Summary: handled 4, deferred 1, not served 0. Final stock: 3 trucks, 140 packs.

### Test Set 2: Insufficient resources (start: 3 trucks, 120 packs)

| Request | Score | Wanted | Action chosen | Trucks used | Packs used | Trucks left | Packs left |
|---|---|---|---|---|---|---|---|
| R1 | 95 | A | A - Full Relief | 2 | 50 | 1 | 70 |
| R2 | 11 | D | D - Defer / Monitor | 0 | 0 | 1 | 70 |
| R3 | 74 | A | B - Standard Relief (fallback) | 1 | 30 | 0 | 40 |
| R4 | 25 | C | Not served | 0 | 0 | 0 | 40 |
| R5 | 74 | A | Not served | 0 | 0 | 0 | 40 |

Summary: handled 2, deferred 1, not served 2. Final stock: 0 trucks, 40 packs.

### Extra checks (start: 5 trucks, 120 packs)

| Check | Input | Result |
|---|---|---|
| Wrong input | Letters, then `9`, then `3` for damage | Asked again, then accepted `3` |
| Standard Relief from score | High, 150 families, Open, n, n, n (score 40) | B - Standard Relief |
| Medical gives at least C | Low, 10 families, Open, n, y, n (score 36) | C - Emergency Kit Only |
| Low score is deferred | Low, 10 families, Open, n, n, y (score -4) | D - Defer / Monitor |
| Full Relief | High, 200 families, Damaged, y, n, n (score 70) | A - Full Relief |
| Fallback with 1 truck left | Same as above, after other requests use stock | B - Standard Relief |

## Known Limits

- The number of requests is fixed at 5 (`TOTAL_REQUESTS` in the code).
- Results are shown on screen only; nothing is saved to a file.
- If the input ends early (Ctrl+D on Linux/macOS, Ctrl+Z then Enter on Windows), the program keeps asking the same question. Press Ctrl+C to stop it.
- The final `system("pause")` line works only on Windows. On Linux/macOS it prints an error. It can be removed; the program works without it.

## Use of AI Tools

We used an AI tool to help us understand the assignment. We then write the code, compiled it, ran both test sets, and made changes ourselves.

## GitHub Repository

https://github.com/aliyan2525/Relief-Distribution-System-Assignment01
