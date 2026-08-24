# C 单向链表

文件说明：

- `linked_list.h`：链表类型和接口声明
- `linked_list.c`：链表接口实现
- `main.c`：基本操作演示

编译运行：

```sh
cc -std=c11 -Wall -Wextra -Wpedantic linked_list.c main.c -o linked_list
./linked_list
```

当前实现包含：

- 头部插入 `list_push_front`
- 尾部插入 `list_push_back`
- 按下标插入 `list_insert_at`
- 按值查找 `list_find`
- 按下标删除 `list_delete_at`
- 遍历打印 `list_print`
- 释放全部节点 `list_clear`
