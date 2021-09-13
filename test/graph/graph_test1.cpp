#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
/*
   Assignmet 3 test file, all the test cases are in this file.
   To understand the rationale behind this test, please go to
   test/README.md to see the details
*/
TEST_CASE("Template tests") {
	// testing the templates works well
	gdwg::graph<std::string, int> g1;
	{
		std::string a{"A"};
		std::string b{"B"};
		std::string c{"C"};
		std::string d{"D"};
		g1.insert_node(a);
		g1.insert_node(b);
		g1.insert_node(c);
		g1.insert_node(d);
	}
	CHECK(g1.nodes() == std::vector<std::string>{"A", "B", "C", "D"});
	CHECK(g1.is_node("F") == false);
	gdwg::graph<double, int> g2;
	g2.insert_node(1.2345);
	g2.insert_node(2.3456);
	g2.insert_node(3.4567);
	CHECK(g2.nodes() == std::vector<double>{1.2345, 2.3456, 3.4567});
	CHECK(g2.is_node(2.222) == false);
	gdwg::graph<int, int> g3;
	g3.insert_node(3);
	g3.insert_node(4);
	g3.insert_node(5);
	CHECK(g3.nodes() == std::vector<int>{3, 4, 5});
	CHECK(g3.is_node(5) == true);
}

TEST_CASE("Output stream test") {
	/*
	   This test case used in this section is
	   originally from ass3/README.md in course
	   COMP6771
	*/
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};

	auto g = graph{};
	g.insert_node(1);

	g.insert_node(2);
	g.insert_node(3);
	g.insert_node(4);
	g.insert_node(5);
	g.insert_node(6);
	for (const auto& x : v) {
		g.insert_edge(x.from, x.to, x.weight);
	};

	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
	CHECK(out.str() == expected_output);
}

TEST_CASE("Nodes and edges tests") {
	/*
	   intialise a graph with
	   "A", "B", 5
	   "A", "C", 10
	   "C", "B", 15
	*/
	gdwg::graph<std::string, int> g1;
	g1.insert_node("A");
	g1.insert_node("B");
	g1.insert_node("C");
	g1.insert_edge("A", "B", 5);
	g1.insert_edge("A", "C", 10);
	g1.insert_edge("C", "B", 15);
	// test replace node
	g1.replace_node("A", "D");
	CHECK(g1.nodes() == std::vector<std::string>{"B", "C", "D"});
	// test weight() functions;
	CHECK(g1.weights("D", "B") == std::vector<int>{5});
	CHECK(g1.weights("C", "B") == std::vector<int>{15});
	// test is_connected()
	CHECK(g1.is_connected("C", "C") == false);
	// test connections()
	CHECK(g1.connections("D") == std::vector<std::string>{"B", "C"});
	// test erase_node() and erase_edge()
	g1.erase_node("D");

	CHECK(g1.is_connected("C", "B") == true);
	g1.erase_edge("C", "B", 15);
	CHECK(g1.is_connected("C", "B") == false);
	g1.erase_node("C");
	g1.erase_node("B");
	CHECK(g1.empty() == true);

	/*
	   Testing merge_replace_node(), the exapmle is from
	   ass3/README.md
	*/
	gdwg::graph<std::string, int> g2;
	g2.insert_node("A");
	g2.insert_node("B");
	g2.insert_node("C");
	g2.insert_node("D");
	g2.insert_edge("A", "B", 1);
	g2.insert_edge("A", "C", 2);
	g2.insert_edge("A", "D", 3);
	auto g2_out = std::ostringstream{};
	g2_out << g2;
	auto const expected_g2_output = std::string_view(R"(A (
  B | 1
  C | 2
  D | 3
)
B (
)
C (
)
D (
)
)");
	CHECK(g2_out.str() == expected_g2_output);
	g2.merge_replace_node("A", "B");
	auto g2_res = std::ostringstream{};
	g2_res << g2;
	auto const expected_g2_res = std::string_view(R"(B (
  B | 1
  C | 2
  D | 3
)
C (
)
D (
)
)");
	CHECK(g2_res.str() == expected_g2_res);
	gdwg::graph<double, double> g3;
	g3.insert_node(1.22);
	g3.insert_node(1.33);
	g3.insert_edge(1.22, 1.33, 2.33);
	g3.insert_edge(1.33, 1.22, 33.33);
	g3.clear();
	CHECK(g3.empty());
}

TEST_CASE("Iterator tests") {
	gdwg::graph<std::string, int> g1;
	g1.insert_node("A");
	g1.insert_node("B");
	g1.insert_node("C");
	g1.insert_node("D");
	g1.insert_edge("A", "B", 1);
	g1.insert_edge("A", "C", 2);
	g1.insert_edge("A", "D", 3);
	g1.insert_edge("B", "D", 133);
	g1.insert_edge("C", "D", 1324);
	// test begin() and end() and find()
	auto it_find = g1.find("A", "B", 1);
	auto it_not_find = g1.find("B", "C", 12);
	CHECK(it_find == g1.begin());
	CHECK(it_not_find == g1.end());
	// test ++it and --it and *it
	++it_find;
	CHECK(((*it_find).from == "A" && (*it_find).to == "C" && (*it_find).weight == 2));
	--it_find;
	CHECK(((*it_find).from == "A" && (*it_find).to == "B" && (*it_find).weight == 1));
	// test erase_edge() and iterator comparision
	auto it_erase = g1.erase_edge(it_find);
	CHECK(it_erase == g1.begin());
	// test erase_edge() with range[i,s)
	auto it_i = g1.begin();
	auto it_s = g1.end();
	++it_i;
	--it_s;

	auto it_erase_range = g1.erase_edge(it_i, it_s);
	CHECK(((*it_erase_range).from == "C" && (*it_erase_range).to == "D"
	       && (*it_erase_range).weight == 1324));
}

TEST_CASE("Constructor tests") {
	// constructed from a vector
	std::vector<int> i{1, 3, 5, 7, 9};
	gdwg::graph<int, int> g1(i.begin(), i.end());
	CHECK(g1.nodes() == i);
	// copy constructor and
	gdwg::graph<int, int> g2(g1);
	CHECK(g2 == g1);
	g2.insert_node(10);
	// copy assignment
	gdwg::graph<int, int> g3 = g1;
	CHECK(g3 == g1);
	g3.insert_node(10);
	// auto operator=(graph&& other) noexcept -> graph&;
	gdwg::graph<int, int> g4 = std::move(g1);
	CHECK(g4.nodes() == i);
	CHECK(g1.empty());
	// graph(graph&& other) noexcept;
	gdwg::graph<int, int> g5(std::move(g4));
	CHECK(g5.nodes() == i);
	CHECK(g4.empty());
}

TEST_CASE("Exception tests") {
	std::vector<int> i{1, 3, 5, 7, 9};
	gdwg::graph<int, int> g1(i.begin(), i.end());
	CHECK_THROWS_MATCHES(g1.insert_edge(2, 4, 5),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge when "
	                                              "either src or dst node does not exist"));
	CHECK_THROWS_MATCHES(g1.replace_node(2, 4),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node on a "
	                                              "node that doesn't exist"));
	CHECK_THROWS_MATCHES(g1.replace_node(2, 4),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node on a "
	                                              "node that doesn't exist"));
	CHECK_THROWS_MATCHES(g1.merge_replace_node(2, 4),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
	                                              "E>::merge_replace_node on old or new data if "
	                                              "they don't exist in the graph"));
	CHECK_THROWS_MATCHES(g1.erase_edge(2, 4, 5),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on src "
	                                              "or dst if they don't exist in the graph"));
	CHECK_THROWS_MATCHES(g1.is_connected(2, 4),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected if "
	                                              "src or dst node don't exist in the graph"));
	CHECK_THROWS_MATCHES(g1.weights(2, 4),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src or "
	                                              "dst node don't exist in the graph"));
	CHECK_THROWS_MATCHES(g1.connections(2),
	                     std::runtime_error,
	                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
	                                              "src doesn't exist in the graph"));
}
