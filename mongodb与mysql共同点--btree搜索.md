mysql 和mongodb 都需要进行 btree 搜索 然后才能获取到数据是吗


• 在使用普通索引的查询中，可以这样理解；但不是所有查询都一定走 B-tree。

  ## 使用索引时

  ### MySQL InnoDB

  如果查询主键：

  SELECT *
  FROM user
  WHERE id = 2;

  大致是：

  主键 B+ 树
      ↓
  找到叶子节点
      ↓
  直接拿到完整数据行

  因为 InnoDB 的聚簇索引叶子节点保存完整行。

  如果查询二级索引：

  SELECT *
  FROM user
  WHERE name = 'Bob';

  大致是：

  name 二级索引树
      ↓
  找到 name='Bob'
  得到主键 id
      ↓
  主键聚簇索引树
      ↓
  获取完整数据行

  这就是回表。

  ### MongoDB

  如果查询字段上有索引：

  db.users.find({ name: "Bob" })

  大致是：

  name 索引树
      ↓
  找到索引 Key
  得到文档定位信息
      ↓
  读取集合中的完整文档

  MongoDB 普通索引通常和集合数据分开，所以找到索引记录后，通常还要读取完整文档。

  ———

  ## 但没有索引时，不一定进行 B-tree 搜索

  例如：

  SELECT *
  FROM user
  WHERE email = 'a@example.com';

  如果 email 没有索引，MySQL 可能进行全表扫描：

  第 1 行 -> 第 2 行 -> 第 3 行 -> ...

  MongoDB 也可能进行集合扫描：

  db.users.find({ email: "a@example.com" })

  大致是：

  第 1 个文档 -> 第 2 个文档 -> 第 3 个文档 -> ...

  这种情况不是先查 B+ 树，而是直接遍历数据。

  ## 覆盖索引时可以少一步

  如果查询所需字段都在索引中，就不需要读取完整数据。

  例如 MongoDB：

  db.users.createIndex({ name: 1, age: 1 });

  db.users.find(
    { name: "Bob" },
    { _id: 0, name: 1, age: 1 }
  );

  可以理解为：

  索引树
    ↓
  直接返回 name、age

  MySQL 也有类似的覆盖索引：

  CREATE INDEX idx_name_age ON user(name, age);

  SELECT name, age
  FROM user
  WHERE name = 'Bob';

  ## 更准确的总结

  有普通索引：
  查询条件
    ↓
  搜索 B-tree/B+Tree 索引
    ↓
  找到数据定位信息
    ↓
  必要时读取完整数据

  没有索引：
  查询条件
    ↓
  扫描整张表或整个集合

  另外，数据库还可能使用其他索引类型，例如哈希索引、全文索引、空间索引等。因此更准确的说法是：

  > MySQL 和 MongoDB 在使用普通有序索引时，通常先通过 B-tree 家族结构定位数据；如果索引中没有完整查询结
  > 果，还需要进一步读取数据。没有合适索引时，则可能退化为全表扫描或全集合扫描。