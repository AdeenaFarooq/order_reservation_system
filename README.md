# ordrly_table_hub

# A. Project Introduction
The Ordrly Table Hub is a console-based program written in C that lets a restaurant take orders in advance. Customers choose a dish from a menu with prices, enter a quantity and an arrival time, and the program calculates the bill and reserves the order. The program supports adding, viewing, editing, cancelling and searching orders, limits how many orders can share one arrival time, validates user input, and saves all orders to a text file so they are not lost when the program closes.

# B. Features and Test Cases

## Case 1: Adding and Viewing an Order
- **Description:** Tests adding a new order from the menu and viewing it with the calculated total.
- **Inputs:** Menu choice `1`, name `noah`, item number`1`, quantity `2`, arrival time `7:50`.
-  **Expected Output:** The menu is displayed, then `Bill: Rs. 900` and `[OK]Order added.` Viewing shows name noah, item Alfredo pasta, quantity 2, total Rs. 900, arrival 7:50.

  ### Case 1B : Time slot Full
  - **Description:** Tests that no more than 3 orders can be placed for the same arrival time.
- **Inputs:** Add three orders with arrival time `7:50`, then try to add a fourth order at `7:50`.
  - **Expected Output:** `Sorry, the entered time slot is full. Try another time.` The fourth order is not saved.
 
 ### Case 1C: Invalid Input and Error Handling
  -  **Description:** Tests how the program handles wrong input.
- **Inputs:** Letters (`abc`) at the main menu; menu choice `9`; item number `9`; quantity `0`.
- **Expected Output:** `Invalid input. Enter a number:`, `Invalid choice, try again.`, `Invalid item number.` and `Quantity must be at least 1.` The user is asked to enter the value again each time.

## Case 2: Viewing Orders
- **Description:** Checks that the program lists every saved order with its details and total.
- **Inputs:** Menu choice `2`, after at least one order has been added.
- **Expected Output:** Each order is printed with its number, name, item, quantity, total in rupees and arrival time.


## Case 3: Cancelling an Order
- **Description:** Tests removing an order and shifting the remaining orders up.
- **Inputs:** Menu choice `3`, order number `2`. Then menu choice `3` with order number `99`.
- **Expected Output:** `[OK]Order cancelled.` and the remaining orders are renumbered. For `99`: `Invalid order number.`

### Case 3B:
**Inputs:** Enter Menu choice`3`, order number `99`.
**Expected Output:** For `99`: `Invalid Order number.`

## Case 4: Editing an Order
- **Description:** Tests changing the quantity of an existing order.
- **Inputs:** Menu choice `4`, order number `3`, new quantity `4`.
- **Expected Output:** `Order updated.` Viewing the order shows quantity 2 and the recalculated total.

## Case 5: Searching by Customer Name
- **Description:** Tests searching for orders by part of a customer name.
- **Inputs:** Menu choice `5`, search text `noah`. 
- **Expected Output:** All orders whose name contains `noah` are displayed.

### Case 5B:
- **Input:** Enter menu choice `5` and name `zed`.
- **Output:** For `zed`: `[!]No matching orders.`

## Case 6: Viewing the Menu
- **Description:** Tests that the restaurant menu with prices can be displayed on its own.
- **Inputs:** Menu choice `6`.
- **Expected Output:** The menu is shown with six dishes and their prices (Alfredo Pasta Rs. 450, Beef Burger Rs. 600, Lemonade Rs. 1200, Fries Rs. 250, Cold Drink Rs. 100, Ice Cream Rs. 200).

 ## Case 7: Exiting the Program
- **Description:** Checks that the program closes properly when the user chooses to exit.
- **Inputs:** Menu choice `0`.
- **Expected Output:** The program prints `Goodbye` and stops.


## Case 8: Saving and Loading Data
- **Description:** Tests that orders are saved to `orders.txt` and loaded again at startup.
- **Inputs:** Add one or more orders, choose `0` to exit, run the program again, then choose `2`.
- **Expected Output:** Screen closes on exit. After restarting, the previously added orders are still displayed.


# Instructions To Clone and Execute

**Step 1: Clone the repository**

Open your terminal and execute:

```
git clone  https://github.com/AdeenaFarooq/ordrly_table_hub.git
cd ordrly_table_hub

```

**Step 2: Compile the code**

```
gcc -std=c99 main.c -o project
```

**Step 3: Run the program**

Windows (Command Prompt):

```
project
```






  
  
