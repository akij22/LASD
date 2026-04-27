#include "linked_list.h"

#include <cstdio>

static int get_address(const void *node, const void *address_ref) {
    return static_cast<int>(
        reinterpret_cast<const char *>(node) -
        reinterpret_cast<const char *>(address_ref)
    );
}

list_t *list_new(bool details) {
    list_t *list = new list_t;

    if (details) {
        std::printf("Lista creata\n");
    }

    list->head = nullptr;

    if (details) {
        std::printf("Imposto a NULL head\n");
    }

    return list;
}

void list_delete(list_t *list) {

    //// implementare rimozione dal fondo della lista
    //// deallocazione struct list
}

void list_insert_front(list_t *list, int elem) {
    node_t *new_node = new node_t;
    new_node->val = elem;
    new_node->next = list->head;
    list->head = new_node;
}

void list_print(const list_t *list, bool details, const void *address_ref) {
    if (list->head == nullptr) {
        std::printf("Lista vuota\n");
        return;
    }

    const node_t *current = list->head;

    while (current != nullptr) {
        if (!details) {
            std::printf("%d, ", current->val);
        } else if (current->next == nullptr) {
            std::printf(
                "allocato in %d [Val: %d, Next: NULL]\n",
                get_address(current, address_ref),
                current->val
            );
        } else {
            std::printf(
                "allocato in %d [Val: %d, Next: %d]\n",
                get_address(current, address_ref),
                current->val,
                get_address(current->next, address_ref)
            );
        }

        current = current->next;
    }

    std::printf("\n");
}
