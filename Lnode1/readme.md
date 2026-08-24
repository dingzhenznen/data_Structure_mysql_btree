# `node.c` 编译与运行

## 进入项目目录

```sh
cd /Users/dingzhenzhen/works/study/mysql_btree/Lnode1
```

## 编译

使用 C11 标准，并开启常用编译警告：

```sh
cc -std=c11 -Wall -Wextra -Wpedantic node.c -o node
```

编译成功后，会在当前目录生成可执行文件 `node`。

## 运行

```sh
./node
```

如果链表节点数据为 `1`、`2`、`3`，运行结果为：

```text
1
2
3
```

## 一键编译并运行

```sh
cc -std=c11 -Wall -Wextra -Wpedantic node.c -o node && ./node
```

## 清理可执行文件

```sh
rm -f node
```
