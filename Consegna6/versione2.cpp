#include <stdio.h>
#include <string.h>

struct Product {
    int code;
    char label[32];
    double unitPrice;
    int stock;
};

class Warehouse {
    Product products[80];
    int count;

    void copyLabel(char *dest, const char *src) {
        int i = 0;
        while (src[i] != 0 && i < 31) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = 0;
    }

    int findPosition(int code) {
        for (int i = 0; i < count; i++) {
            if (products[i].code == code) {
                return i;
            }
        }
        return -1;
    }

    double taxFor(Product product) {
        if (product.unitPrice > 120.0) {
            return product.unitPrice * 0.22;
        }
        if (product.unitPrice > 20.0) {
            return product.unitPrice * 0.10;
        } else {
            return product.unitPrice * 0.04;
        }
    }

public:
    Warehouse() {
        count = 0;
    }

    int size() {
        return count;
    }

    void addProduct(int code, const char *label, double unitPrice, int stock) {
        if (count >= 80) {
            return;
        }
        products[count].code = code;
        copyLabel(products[count].label, label);
        products[count].unitPrice = unitPrice;
        products[count].stock = stock;
        count++;
    }

    bool removeProduct(int code) {
        int position = findPosition(code);
        if (position < 0) {
            return false;
        }
        for (int i = position; i < count - 1; i++) {
            products[i] = products[i + 1];
        }
        count--;
        return true;
    }

    bool sell(int code, int amount) {
        int position = findPosition(code);
        if (position < 0 || amount <= 0) {
            return false;
        }
        if (products[position].stock < amount) {
            return false;
        }
        products[position].stock -= amount;
        return true;
    }

    double totalValue() {
        double total = 0.0;
        for (int i = 0; i < count; i++) {
            Product product = products[i];
            total += product.unitPrice * product.stock;
            total += taxFor(product);
        }
        return total;
    }

    void printLowStock(int limit) {
        for (int i = 0; i < count; i++) {
            Product product = products[i];
            if (product.stock <= limit) {
                printf("%d %s\n", product.code, product.label);
            }
        }
    }

    void applyDiscount(double percent) {
        if (percent <= 0.0 || percent >= 1.0) {
            return;
        }
        for (int i = 0; i < count; i++) {
            products[i].unitPrice = products[i].unitPrice * (1.0 - percent);
        }
    }

    void refill(int code, int value) {
        int position = findPosition(code);
        if (position < 0) {
            return;
        }
        if (value > 0) {
            products[position].stock += value;
        }
    }

    double averagePrice() {
        if (count == 0) {
            return 0.0;
        }
        double total = 0.0;
        for (int i = 0; i < count; i++) {
            total += products[i].unitPrice;
        }
        return total / count;
    }

    void removeEmpty() {
        int write = 0;
        for (int i = 0; i < count; i++) {
            if (products[i].stock > 0) {
                products[write] = products[i];
                write++;
            }
        }
        count = write;
    }

    void printAll() {
        for (int i = 0; i < count; i++) {
            printf("%d %s %.2f %d\n",
                   products[i].code,
                   products[i].label,
                   products[i].unitPrice,
                   products[i].stock);
        }
    }

    int countExpensive(double threshold) {
        int counter = 0;
        for (int i = 0; i < count; i++) {
            if (products[i].unitPrice > threshold) {
                counter++;
            }
        }
        return counter;
    }
};

int main() {
    Warehouse warehouse;
    warehouse.addProduct(10, "keyboard", 49.9, 12);
    warehouse.addProduct(11, "monitor", 249.0, 5);
    warehouse.addProduct(12, "cable", 7.5, 40);
    warehouse.sell(10, 2);
    warehouse.applyDiscount(0.05);
    warehouse.refill(11, 3);
    warehouse.printLowStock(10);
    warehouse.removeEmpty();
    warehouse.printAll();
    printf("%.2f\n", warehouse.averagePrice());
    printf("%.2f\n", warehouse.totalValue());
    printf("%d\n", warehouse.countExpensive(50.0));
    return 0;
}
