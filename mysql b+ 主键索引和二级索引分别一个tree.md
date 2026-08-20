mysql b+tree ,对于二级索引是一个单独的b+ 树吗，和 主键索引树是两棵树？


• 是的。InnoDB 中每个索引通常都是一棵独立的 B+ 树，所以二级索引树和主键索引树是两棵不同的树。

  ## 示例

  CREATE TABLE user (
      id BIGINT PRIMARY KEY,
      name VARCHAR(100),
      age INT,
      INDEX idx_name(name)
  );

  这张表至少有两棵 B+ 树：

  1. 主键索引树，也叫聚簇索引树
     按 id 排序

  2. 二级索引树
     按 name 排序

  ## 主键索引树(叶子节点存储完整数据行，数据和索引在一起，所以叫聚簇索)

  叶子节点保存完整数据行：

  主键索引：

  [id=1, name='Alice', age=20]
  [id=2, name='Bob',   age=30]
  [id=3, name='Carol', age=25]

  ## 二级索引树

  二级索引叶子节点通常保存：

  二级索引：

  name='Alice' -> id=1
  name='Bob'   -> id=2
  name='Carol' -> id=3

  这里的 id 是主键值，也叫索引记录指针。InnoDB 通常不是保存物理磁盘地址，而是保存主键值。

  ## 查询过程

  执行：

  SELECT *
  FROM user
  WHERE name = 'Bob';

  过程是：

  二级索引树
      ↓ 找到 name='Bob'
  得到主键 id=2
      ↓
  主键聚簇索引树
      ↓
  找到完整数据行

  这个从二级索引找到主键，再到聚簇索引查完整数据的过程叫：

  > 回表

  因此，这条查询通常需要访问两棵 B+ 树。

  ## 主键查询不需要回表

  执行：

  SELECT *
  FROM user
  WHERE id = 2;

  直接查主键聚簇索引树：

  主键索引树
      ↓
  完整数据行

  因为聚簇索引的叶子节点本身就保存完整数据。

  ## 覆盖索引可以避免回表

  例如：

  CREATE INDEX idx_name_age ON user(name, age);

  执行：

  SELECT name, age
  FROM user
  WHERE name = 'Bob';

  二级索引中已经包含查询所需的 name 和 age，因此可以直接返回结果，不必再访问主键索引树：

  二级索引树
      ↓
  直接得到 name、age

  这种情况叫覆盖索引。

  ## 一个重要细节

  如果表没有显式主键，InnoDB 会依次选择：

  1. 第一个定义为 PRIMARY KEY 的索引；
  2. 第一个所有列都声明为 NOT NULL 的唯一索引；
  3. 自动生成隐藏的聚簇索引。

  所以总结为：

  主键索引树：按主键组织，叶子节点保存完整数据行

  二级索引树：按索引列组织，叶子节点保存索引列和主键值
                           ↓
                      可能需要回表

  但它们不是完全互相独立的：二级索引依赖主键值定位聚簇索引中的数据