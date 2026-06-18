#include <stdio.h>
#include <string.h>

struct Item {
    int id;
    char name[32];
    double price;
    int quantity;
};

class Inventory {
    Item items[80];
    int used;

    void copyName(char *dest, const char *src) {
        int i = 0;
        while (src[i] != 0 && i < 31) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = 0;
    }

    int findIndex(int id) {
        for (int i = 0; i < used; i++) {
            if (items[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    double taxFor(Item item) {
        if (item.price > 100.0) {
            return item.price * 0.22;
        }
        if (item.price > 20.0) {
            return item.price * 0.10;
        } else {
            return item.price * 0.04;
        }
    }

public:
    Inventory() {
        used = 0;
    }

    int size() {
        return used;
    }

    void addItem(int id, const char *name, double price, int quantity) {
        if (used >= 80) {
            return;
        }
        items[used].id = id;
        copyName(items[used].name, name);
        items[used].price = price;
        items[used].quantity = quantity;
        used++;
    }

    bool removeItem(int id) {
        int pos = findIndex(id);
        if (pos < 0) {
            return false;
        }
        for (int i = pos; i < used - 1; i++) {
            items[i] = items[i + 1];
        }
        used--;
        return true;
    }

    bool sell(int id, int count) {
        int pos = findIndex(id);
        if (pos < 0 || count <= 0) {
            return false;
        }
        if (items[pos].quantity < count) {
            return false;
        }
        items[pos].quantity -= count;
        return true;
    }

    double totalValue() {
        double total = 0.0;
        for (int i = 0; i < used; i++) {
            Item item = items[i];
            total += item.price * item.quantity;
            total += taxFor(item);
        }
        return total;
    }

    void printLowStock(int limit) {
        for (int i = 0; i < used; i++) {
            Item item = items[i];
            if (item.quantity <= limit) {
                printf("%d %s\n", item.id, item.name);
            }
        }
    }

    void applyDiscount(double percent) {
        if (percent <= 0.0 || percent >= 1.0) {
            return;
        }
        for (int i = 0; i < used; i++) {
            items[i].price = items[i].price * (1.0 - percent);
        }
    }

    void restock(int id, int value) {
        int pos = findIndex(id);
        if (pos < 0) {
            return;
        }
        if (value > 0) {
            items[pos].quantity += value;
        }
    }

    void printLegacyReport() {
        printf("legacy report\n");
        for (int i = 0; i < used; i++) {
            if (items[i].quantity > 0) {
                printf("%d\n", items[i].id);
            }
        }
    }

    void printAll() {
        for (int i = 0; i < used; i++) {
            printf("%d %s %.2f %d\n",
                   items[i].id,
                   items[i].name,
                   items[i].price,
                   items[i].quantity);
        }
    }

    int countExpensive(double threshold) {
        int count = 0;
        for (int i = 0; i < used; i++) {
            if (items[i].price > threshold) {
                count++;
            }
        }
        return count;
    }
};

int main() {
    Inventory inventory;
    inventory.addItem(10, "keyboard", 49.9, 12);
    inventory.addItem(11, "monitor", 219.0, 5);
    inventory.addItem(12, "cable", 7.5, 40);
    inventory.sell(10, 2);
    inventory.applyDiscount(0.05);
    inventory.restock(11, 3);
    inventory.printLowStock(10);
    inventory.printLegacyReport();
    inventory.printAll();
    printf("%.2f\n", inventory.totalValue());
    printf("%d\n", inventory.countExpensive(50.0));
    return 0;
}

int obsoleteMetric(int value) {
    int result = 0;
    for (int i = 0; i < value; i++) {
        result += i;
    }
    return result;
}
