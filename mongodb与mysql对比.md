MongoDB 相对 MySQL 的查询优势，主要不是 B+ 树更快，而是数据模型不同，某些查询可以少做关联、少做拆分。

  ## 1. 嵌套数据可以一次读取

  MongoDB 可以把相关数据直接嵌入一个文档：

  {
    _id: 1,
    name: "订单 1",
    customer: {
      name: "Alice",
      phone: "123456"
    },
    items: [
      { product: "Book", count: 2 },
      { product: "Pen", count: 5 }
    ]
  }

  查询订单时，客户信息和商品明细已经在同一个文档里：

  db.orders.findOne({ _id: 1 })

  不需要像关系型数据库那样拆成多张表后再 JOIN。MongoDB 官方将嵌入模型列为减少
  I/O、通过一次操作获取相关数据的方式。(mongodb.com
  (https://www.mongodb.com/docs/manual/introduction/index.html?utm_source=openai))

  MySQL 中可能需要：

  SELECT *
  FROM orders o
  JOIN customers c ON o.customer_id = c.id
  JOIN order_items i ON i.order_id = o.id
  WHERE o.id = 1;

  因此，对于“经常一起读取的聚合数据”，MongoDB 可能更简单、更快。

  ## 2. 对嵌套字段和数组查询比较自然

  MongoDB 可以直接查询嵌套字段：

  db.users.find({
    "address.city": "Shanghai"
  })

  也可以查询数组中的元素：

  db.products.find({
    "tags": "database"
  })

  并且可以对嵌套字段和数组字段建立索引。(mongodb.com
  (https://www.mongodb.com/docs/manual/introduction/index.html?utm_source=openai))

  这类数据在 MySQL 中通常需要：

  - 拆分成多张表；
  - 使用关联表；
  - 或使用 JSON 字段和 JSON 索引；
  - 查询语句也可能更复杂。

  ## 3. 文档模型更适合聚合查询

  MongoDB 使用聚合管道处理数据：

  db.orders.aggregate([
    { $match: { status: "paid" } },
    { $group: {
        _id: "$customerId",
        total: { $sum: "$amount" }
    }},
    { $sort: { total: -1 } }
  ])

  它适合对文档、数组、嵌套结构进行逐阶段处理。

  MongoDB 也提供 $lookup、$graphLookup 等阶段处理跨集合关联，但如果经常需要复杂关联，MongoDB
  官方建议仔细考虑是否应该使用嵌入模型或引用模型。(mongodb.com
  (https://www.mongodb.com/docs/manual/data-modeling/referencing/?utm_source=openai))

  ## 4. Schema 更灵活，适合字段变化频繁的数据

  同一个集合中的文档不一定必须拥有完全相同的字段：

  { name: "Alice", age: 20 }

  { name: "Bob", age: 30, email: "bob@example.com" }

  { name: "Carol", skills: ["JavaScript", "MongoDB"] }

  这对以下查询场景比较方便：

  - 日志；
  - 用户画像；
  - 商品属性；
  - 配置数据；
  - 迭代频繁的业务对象。

  MongoDB 的灵活模型允许不同文档拥有不同字段，但正式生产环境仍然应该设计合理的数据模型，必要时使用 schema
  validation。(mongodb.com
  (https://www.mongodb.com/docs/v5.1/core/data-modeling-introduction/?utm_source=openai))

  ## 5. 分片和横向扩展更适合大规模数据

  MongoDB 原生支持通过 Sharding 将数据分布到多个节点：

  Shard 1：用户 ID 1 ~ 1000000
  Shard 2：用户 ID 1000001 ~ 2000000
  Shard 3：用户 ID 2000001 ~ 3000000

  查询包含分片键时，可以尽量只访问相关分片；不包含分片键时，可能需要访问多个分片。

  MongoDB
  官方将分片列为其横向扩展能力的一部分，但分片键设计不合理时，也可能导致查询效果很差。(mongodb.com
  (https://www.mongodb.com/docs/manual/introduction/index.html?utm_source=openai))

  ## 6. 但 MongoDB 不一定比 MySQL 查询快

  如果数据本身是强关系结构：

  用户
    ↓
  订单
    ↓
  订单明细
    ↓
  商品

  并且经常进行：

  - 多表关联；
  - 复杂条件组合；
  - 事务查询；
  - 精确聚合；
  - 参照完整性校验；

  MySQL 往往更自然。MySQL 原生支持多表
  JOIN，并且优化器会针对连接、索引、排序和范围查询进行优化。(dev.mysql.com
  (https://dev.mysql.com/doc/refman/8.4/en/join.html?utm_source=openai))

  MongoDB 如果使用引用模型，也需要通过多次查询或 $lookup 关联数据；MongoDB
  官方也说明，引用关系通常意味着应用需要额外读取相关文档。(mongodb.com
  (https://www.mongodb.com/docs/v7.2/data-modeling/concepts/embedding-vs-references/?utm_source=openai))

  ## 对比总结

  ┌──────────────────────────┬───────────────────┐
  │ 场景                     │ 更适合的数据库    │
  ├──────────────────────────┼───────────────────┤
  │ 相关数据经常一起读取     │ MongoDB           │
  │ 嵌套对象和数组查询       │ MongoDB           │
  │ 字段结构经常变化         │ MongoDB           │
  │ 日志、商品属性、用户画像 │ MongoDB           │
  │ 多表关联查询             │ MySQL             │
  │ 复杂报表和关系型聚合     │ 通常 MySQL 更自然 │
  └──────────────────────────┴───────────────────┘

  一句话总结：

  > MongoDB 的查询优势主要来自“文档内聚合”：相关数据可以嵌套在同一个文档里，从而减少 JOIN 和多次查询；
  > MySQL 的优势则是关系建模、复杂 JOIN、事务和结构化查询。不能简单说 MongoDB 查询一定比 MySQL 快，最终取
  > 决于数据模型、索引、查询方式和数据规模。