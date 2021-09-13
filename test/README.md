# Assignment 3 Test File Guide
This is a test rationale description behind test strategy in Assignment 3. All the test code will be located at `${workspaceFolder}/test/graph/graph_test1.cpp`. The test code will contain following sections to make sure the requirements in Assignment 3 spec are fully and successfully implemented.
- Template tests
- Output stream tests
- Nodes and edges tests
- Iterator tests
- Constructor tests
- Exception tests

## Template tests
In this section, the tests will focus on different data types a graph may take since it is a template. The graph may be defined with some basic data type like `int`,`double`,`std::string` and so on. The tests will insure the graph class is implemented well in terms of `typename`.
Because tests in this section requires know the nodes and weights, so there function will be test as well(**for convinience, the return type and parameter are omitted in all funtions mentioned in this file**).
- `is_node()`
- `nodes()`
- `insert_node()`
## Output stream tests
The output stream test is prior to nodes and edges tests because we need to get the exactly nodes and edges in a graph in next section. This section will be testing the correctnesss of output function.


## Nodes and edges tests
In this section, the test cases are involved in the fundamental but also most important parts of this assignment, that is, **manipulating nodes and edges in a graph and get some properties in a graph**.
The tests will focus on these functions:
- `insert_edge()`
- `replace_node()`
- `merge_replace_node()`
- `erase_node()`
- `erase_edge()`
- `is_connected()`
- `weight()`
- `connections()`
- `empty()`
- `clear()`


## Iterator tests
This section is testing the iterator class works properly. It contains testing the iterator dereference, constructor, traversal and comparison.
For a iterator `it` of class graph, it has follwing operations.
- `++it` or `it++`
- `--it` or `it--`
- `*it`
- iterator comparision

Besides, this section will also be testing functions that return a iterator class, like
- `begin()`
- `end()`
- `find()`
- `erase_edge()`

## Constructor tests
This section includes test cases about graph constructor test, the major parts are constructed from `InputIt` and copy assignment and copy constructor. Also `==` comparision will be test in this section.

## Exception testes
Test functions that may throw a exception, they are
- `auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;`
- `auto replace_node(N const& old_data, N const& new_data) -> bool;`
- `auto merge_replace_node(N const& old_data, N const& new_data) -> void;`
- `auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;`
- `[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool;`
- `[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E>;`
- `[[nodiscard]] auto connections(N const& src) -> std::vector<N>;`
