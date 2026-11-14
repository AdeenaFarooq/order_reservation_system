#include <stdio.h>
#include <string.h>

#define MAX_ORDERS 100
#define MAX_PER_SLOT 3
#define FILE_NAME "orders.txt"
#define MENU_SIZE 6

struct order{
	char name[50]; 
	char item[50]; //holds food item string
	int quantity; //how many items ordered
	char arrivaltime[10]; //holds time string eg 7:30
};



//parallel arrays: the item at position 0 in names has the price at position 0 in prices
const char *menunames[MENU_SIZE] = {
	"Alfredo pasta", "Beef Burger", "pizza",
	"Fries", "lemonade", "Ice Cream"
};
const int menuprices[MENU_SIZE] = {450, 600, 1200, 250, 100, 200};

void showmenu(){
	printf("\n---------- MENU ----------\n");
	for(int i = 0; i < MENU_SIZE; i++){
		printf("%d. %-18s Rs. %d\n", i + 1, menunames[i], menuprices[i]);
	}
	printf("--------------------------\n");
}


int getprice(const char item[]){
	for(int i = 0; i < MENU_SIZE; i++){
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

int readint(const char *prompt){ //reutrns a validated integer
	int value;
	printf("%s", prompt);
	while(scanf("%d", &value) != 1){ //if user types letters instead scanf fails
		printf("Invalid input. Enter a number: ");
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
	FILE *fp = fopen(FILE_NAME, "w"); //gets a file pointer handle
	if (fp == NULL){ //checks if file failed to open
		printf("Error: could not save to file.\n");
		return;
	}
	
	for(int i = 0; i < count; i++){
		fprintf(fp, "%s|%s|%d|%s\n", orders[i].name, orders[i].item, orders[i].quantity, orders[i].arrivaltime);
		
	} //prints data into file seperated by |
	fclose(fp); //closes file
}

void loadorders(struct order orders[], int *count){
	FILE *fp = fopen(FILE_NAME, "r");
	if (fp == NULL) return;
	
	char line[200]; //hold one full line of text from file at a time
	while(*count < MAX_ORDERS && fgets(line, sizeof(line), fp) != NULL){
		struct order o; //creates empty order box
		
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
		if(strcmp(orders[i].arrivaltime, time) == 0){ //0 means same text
			total++;
		}
	}
	return total;
}

void printorder(struct order o, int number){
	printf("\nOrder %d\n", number);
	printf("Name: %s\n", o.name);
	printf("Item: %s\n", o.item);
	printf("Quantity: %d\n", o.quantity);
		printf("Total: Rs. %d\n", getprice(o.item) * o.quantity);
	printf("Arrival: %s\n", o.arrivaltime);
}

void vieworders(struct order orders[], int count){
	if(count == 0){
		printf("No orders yet.\n");
		return;
	}
	for(int i = 0; i < count; i++){
		printorder(orders[i], i + 1);
	}
}

void addorder(struct order orders[], int *count){
	if(*count >= MAX_ORDERS){
		printf("Order list is full.\n");
		return;
	}

	struct order o; //temporary box to hold new order before validation
	readtext("Customer name: ", o.name, sizeof(o.name));
    	showmenu();
	int pick;
	do{
		pick = readint("Choose item number: ");
		if(pick < 1 || pick > MENU_SIZE){
			printf("Invalid item number.\n");
		}
	} while(pick < 1 || pick > MENU_SIZE);
	strcpy(o.item, menunames[pick - 1]); //copy the chosen name into the order

	do{
		o.quantity = readint("Quantity: ");
		if(o.quantity <= 0){
			printf("Quantity must be at least 1.\n");
		}
	} while(o.quantity <= 0);

	readtext("Arrival time (e.g. 7:30): ", o.arrivaltime, sizeof(o.arrivaltime));

	if(countattime(orders, *count, o.arrivaltime) >= MAX_PER_SLOT){ //rejects if slot has 3 orders
		printf("Sorry, that time slot is full. Try another time.\n");
		return;
	}

	orders[*count] = o;
	(*count)++;
	saveorders(orders, *count);
		printf("Bill: Rs. %d\n", getprice(o.item) * o.quantity);
	printf("Order added.\n");
}

void cancelorder(struct order orders[], int *count){
	if(*count == 0){
		printf("No orders to cancel.\n");
		return;
	}

	vieworders(orders, *count);
	int num = readint("\nOrder number to cancel: ");
	if(num < 1 || num > *count){
		printf("Invalid order number.\n");
		return;
	}

	for(int i = num - 1; i < *count - 1; i++){
		orders[i] = orders[i + 1]; //shift left over the cancelled order
	}
	(*count)--;
	saveorders(orders, *count);
	printf("Order cancelled.\n");
}

void editorder(struct order orders[], int count){
	if(count == 0){
		printf("No orders to edit.\n");
		return;
	}

	vieworders(orders, count);
	int num = readint("\nOrder number to edit: ");
	if(num < 1 || num > count){
		printf("Invalid order number.\n");
		return;
	}

	int q;
	do{
		q = readint("New quantity: ");
		if(q <= 0){
			printf("Quantity must be at least 1.\n");
		}
	} while(q <= 0);

	orders[num - 1].quantity = q;
	saveorders(orders, count);
	printf("Order updated.\n");
}

void searchorders(struct order orders[], int count){
	char key[50];
	readtext("Search customer name: ", key, sizeof(key));

	int found = 0;
	for(int i = 0; i < count; i++){
		if(strstr(orders[i].name, key) != NULL){ //does the name contain the text?
			printorder(orders[i], i + 1);
			found = 1;
		}
	}
	if(!found){
		printf("No matching orders.\n");
	}
}

int main(){
	struct order orders[MAX_ORDERS];
	int count = 0;
	int choice;

	loadorders(orders, &count);

	do{
		printf("\n===== Order Reservation =====\n");
		printf("1. Add order\n");
		printf("2. View orders\n");
		printf("3. Cancel order\n");
		printf("4. Edit order quantity\n");
		printf("5. Search by name\n");
		printf("6. View menu\n");
		printf("0. Exit\n");
		choice = readint("Enter choice: ");

		switch(choice){
			case 1: addorder(orders, &count); break;
			case 2: vieworders(orders, count); break;
			case 3: cancelorder(orders, &count); break;
			case 4: editorder(orders, count); break;
			case 5: searchorders(orders, count); break;
			case 6: showmenu(); break;
			case 0: printf("Goodbye\n"); break;
			default: printf("Invalid choice, try again.\n");
		}
	} while(choice != 0);

	return 0;
}


