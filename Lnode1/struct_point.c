#include <stdio.h>
#include <stdlib.h>
 
struct Books
{
   int   book_id;
};

/* 函数声明 */
void printBook( struct Books *book );
int main(void)
{
    struct Books *Book1 = malloc(sizeof(struct Books));

    if (Book1 == NULL) {
        return 1;
    }

    printf("Book1 变量的地址： %p\n", Book1);

    Book1->book_id = 6495700;

    printBook(Book1);

    free(Book1);
    return 0;
}
void printBook( struct Books *book )
{

   printf( "Book book_id : %d\n", book->book_id);
}