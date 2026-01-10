# 1. Research about vector
## 1.1. std::vector memory usage
research.cpp verify where std::vector store its element (stack or heap) by comparing the heap and stack between two TCs:
- TC1: large number of elements (100000)
- TC2: small number of elements (10)
- Read /proc/selfsmaps to show the usage memory


![TC1 result](data-stcutures/vector/images/TC1.png)

![TC2 result](data-stcutures/vector/images/TC2.png)

=> std::vectors store its element on heap region.