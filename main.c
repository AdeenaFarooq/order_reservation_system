#include <stdio.h> 
#include <string.h>

#define MAXORDERS 100
#define MAXPERSLOT 3
#define FILENAME "orders.txt"
#define MENUSIZE 6
#define WIDTH 52 


#define RESET 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define PINK 5
#define CYAN 6
#define LIGHTPINK 7

struct order{
	char name[50]; 
	char item[50]; // array for storing food items
	int quantity; // this is for quantity of an order
	char arrivaltime[10]; // for time
};

//these are like parallel arrays
const char *menunames[MENUSIZE] = {  // stores a string
	"Alfredo pasta", "Beef Burger", "pizza",
	"Fries", "lemonade", "Ice Cream"
};
const int menuprices[MENUSIZE] = {450, 600, 1200, 250, 100, 200};

// these are main option the frm which the user will choose
const char *mainoptions[6] = {
	"Add order", "View orders", "Cancel order",
	"Edit order quantity", "Search by name", "View menu"
};



void setcolor(int c){ 
	switch(c){
		case RED: printf("\033[91m"); break;
		case GREEN: printf("\033[92m"); break;
		case YELLOW: printf("\033[93m"); break;
		case BLUE: printf("\033[94m"); break;
		case PINK: printf("\033[95m"); break;
		case CYAN: printf("\033[96m"); break;
		case LIGHTPINK: printf("\033[38;5;218m"); break;
		default: printf("\033[0m"); //RESET goes back to normal text
	}
}

void printline(char c){ // prints a blue line
	setcolor(BLUE);
	for(int i = 0; i < WIDTH; i++){
		printf("%c", c);
	}
	setcolor(RESET);
	printf("\n");
}

void printheader(const char *title){ 
	int pad = (WIDTH - strlen(title)) / 2; 
	printf("\n");
	printline('=');
	for(int i = 0; i < pad; i++){
		printf(" ");
	}
	setcolor(YELLOW);
	printf("%s\n", title);
	setcolor(RESET);
	printline('=');
}

void printbanner(){ //it like prints banner kinda
	setcolor(BLUE);
	printf("\n+--------------------------------------------------+\n");
	printf("|                                                  |\n");
	printf("|");
	setcolor(YELLOW);
	printf("                ORDER RESERVATION                 ");
	setcolor(BLUE);
	printf("|\n");
	printf("|     Book the food you love and arrival time      |\n");
	printf("|                                                  |\n");
	printf("+--------------------------------------------------+\n");
	setcolor(RESET);
}

void warn(const char *msg){ //prints a red msg
	printf("  ");
	setcolor(RED);
	printf("[!]"); // will print in red
	setcolor(RESET);
	printf(" %s\n", msg); // this prints msg
}

void ok(const char *msg){ //it will print green ok msg
	printf("  ");
	setcolor(GREEN);
	printf("[OK]");
	setcolor(RESET);
	printf(" %s\n", msg);
}

void printtablehead(){
	setcolor(CYAN);
	printf("%-3s %-14s %-14s %-4s %-8s %-6s\n", "No", "Name", "Item", "Qty", "Total", "Time");
	setcolor(RESET);
	printline('-'); // will call function
}



void showmenu(){ //function which will show menu
	printheader("MENU"); // will call function
	setcolor(PINK);
	for(int i = 0; i < MENUSIZE; i++){
		printf("  %d.  %-20s Rs. %d\n", i + 1, menunames[i], menuprices[i]);
	}
	setcolor(RESET);
	printline('-');
}

int getprice(const char item[]){ //will take an item and compare the name
	for(int i = 0; i < MENUSIZE; i++){
		if(strcmp(menunames[i], item) == 0){
			return menuprices[i];
		}
	}
	return 0;
}
void clearinput(){ //function that returns nothing
	int c;
	while ((c = getchar()) != '\n' && c!= EOF); //sweeps away leftover garbage when hits newline or end-of-file
	
}

int readint(const char *prompt){ //this function makes sure that int is typed
	int value;
	printf("%s", prompt);
	while(scanf("%d", &value) != 1){ // if user types letter so it fails
		setcolor(RED);
		printf(" Invalid input! Enter a number: ");
		setcolor(RESET);
		clearinput();
		
	}
	
	clearinput(); //wipes out the error
	return value; //hands back clean verified number
}

void readtext(const char *prompt, char buffer[], int size){ //reads text string
	printf("%s", prompt);
	
	if(fgets(buffer, size, stdin)==NULL){ //pulls up to size characters fom keyboard into buffer
		buffer[0] = '\0';
		
	}
	else{
		buffer[strcspn(buffer, "\n")] = '\0'; //converts new line into null terminator
		
	}
}
void saveorders(struct order orders[], int count){
	FILE *fp = fopen(FILENAME, "w"); //will open file in write mode
	if (fp == NULL){ // checks if file opened or not
		warn("Could not save the file.");
		return;
	}
	
	for(int i = 0; i < count; i++){
		fprintf(fp, "%s|%s|%d|%s\n", orders[i].name, orders[i].item, orders[i].quantity, orders[i].arrivaltime);
		
	} // this will print data seperated by |
	fclose(fp); // will close the file
}

void loadorders(struct order orders[], int *count){
	FILE *fp = fopen(FILENAME, "r"); // now opening the file for reading
	if (fp == NULL) return;
	
	char line[200]; 
	while(*count < MAXORDERS && fgets(line, sizeof(line), fp) != NULL){
		struct order o; //will create empty order box
		
		if(sscanf(line, "%49[^|]|%49[^|]|%d|%9s", o.name, o.item, &o.quantity, o.arrivaltime) == 4){
			orders[*count] = o; 
			(*count)++;
		}
	}
	fclose(fp);
}

int countattime(struct order orders[], int count, const char time[]){
	int total = 0;
	for(int i = 0; i < count; i++){
		if(strcmp(orders[i].arrivaltime, time) == 0){ // checks if it matches
			total++;
		}
	}
	return total;
}

void printorder(struct order o, int number){ //prints one order as one table row
	setcolor(LIGHTPINK);
	printf("%-3d %-14s %-14s %-4d %-8d %-6s\n", number, o.name, o.item, o.quantity,
	       getprice(o.item) * o.quantity, o.arrivaltime);
	setcolor(RESET);
}

void vieworders(struct order orders[], int count){
	printheader("ORDERS");
	if(count == 0){
		printf("  No orders yet.\n");
		printline('-');
		return;
	}
	printtablehead();
	for(int i = 0; i < count; i++){
		printorder(orders[i], i + 1);
	}
	printline('-');
}

void addorder(struct order orders[], int *count){
	if(*count >= MAXORDERS){
		warn("Order list full !.");
		return;
	}

	struct order o; 
	printheader("NEW ORDER");
	readtext("  Customer name: ", o.name, sizeof(o.name));
	showmenu();
	int pick;
	do{
		pick = readint("  Choose item number: ");
		if(pick < 1 || pick > MENUSIZE){
			warn("Invalid item number.");
		}
	} while(pick < 1 || pick > MENUSIZE);
	strcpy(o.item, menunames[pick - 1]); //copy the chosen name into the order

	do{
		o.quantity = readint("  Quantity: ");
		if(o.quantity <= 0){
			warn("Quantity must be at least 1.");
		}
	} while(o.quantity <= 0);

	readtext("  Arrival time (e.g. 7:30): ", o.arrivaltime, sizeof(o.arrivaltime));

	if(countattime(orders, *count, o.arrivaltime) >= MAXPERSLOT){ //rejects if slot has 3 orders
		warn("Sorry, the entered time slot is full. Try another time.");
		return;
	}

	orders[*count] = o;
	(*count)++;
	saveorders(orders, *count);
	setcolor(YELLOW);
	printf("\n  Bill: Rs. %d\n", getprice(o.item) * o.quantity);
	setcolor(RESET);
	ok("Order added.");
}

void cancelorder(struct order orders[], int *count){
	if(*count == 0){
		warn("No orders to cancel.");
		return;
	}

	vieworders(orders, *count);
	int num = readint("  Order number to cancel: ");
	if(num < 1 || num > *count){
		warn("Invalid order number.");
		return;
	}

	for(int i = num - 1; i < *count - 1; i++){
		orders[i] = orders[i + 1]; //shift left over the cancelled order
	}
	(*count)--;
	saveorders(orders, *count);
	ok("Order cancelled.");
}

void editorder(struct order orders[], int count){
	if(count == 0){
		warn("No orders to edit.");
		return;
	}

	vieworders(orders, count);
	int num = readint("  Order number to edit: ");
	if(num < 1 || num > count){
		warn("Invalid order number.");
		return;
	}

	int q;
	do{
		q = readint("  New quantity: ");
		if(q <= 0){
			warn("Quantity must be at least 1.");
		}
	} while(q <= 0);

	orders[num - 1].quantity = q;
	saveorders(orders, count);
	ok("Order updated.");
}

void searchorders(struct order orders[], int count){
	char key[50];
	printheader("SEARCH");
	readtext("  Search customer name: ", key, sizeof(key));

	int found = 0;
	for(int i = 0; i < count; i++){
		if(strstr(orders[i].name, key) != NULL){ 
			if(!found){ 
				printline('-');
				printtablehead();
			}
			printorder(orders[i], i + 1);
			found = 1;
		}
	}
	if(!found){
		warn("No matching orders.");
	}
	else{
		printline('-');
	}
}

int main(){
	struct order orders[MAXORDERS];
	int count = 0;
	int choice;

	loadorders(orders, &count);
	printbanner();

	do{
		printheader("MAIN MENU");
		printf("  Orders booked: %d / %d\n\n", count, MAXORDERS);
		for(int i = 0; i < 6; i++){ //prints options 1 to 6 from the array
			printf("  ");
			setcolor(CYAN);
			printf("%d", i + 1);
			setcolor(RESET);
			printf(".  %s\n", mainoptions[i]);
		}
		printf("  ");
		setcolor(CYAN);
		printf("0");
		setcolor(RESET);
		printf(".  Exit\n");
		printline('-');
		choice = readint("  Enter choice: ");

		switch(choice){
			case 1: addorder(orders, &count); break;
			case 2: vieworders(orders, count); break;
			case 3: cancelorder(orders, &count); break;
			case 4: editorder(orders, count); break;
			case 5: searchorders(orders, count); break;
			case 6: showmenu(); break;
			case 0: printf("\n  Thank you! See you soon.\n\n"); break;
			default: warn("Invalid choice, try again.");
		}
	} while(choice != 0);

	return 0;
}
