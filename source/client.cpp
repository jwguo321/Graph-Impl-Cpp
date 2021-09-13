#include "gdwg/graph.hpp"
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

auto main() -> int {
	// auto g = gdwg::graph<std::string, int>{};
	// g.insert_node("hello");
	// g.insert_node("how");
	// g.insert_node("are");
	// g.insert_node("you?");

	// g.insert_edge("hello", "how", 5);
	// g.insert_edge("hello", "are", 8);
	// g.insert_edge("hello", "are", 2);

	// g.insert_edge("how", "you?", 1);
	// g.insert_edge("how", "hello", 4);

	// g.insert_edge("are", "you?", 3);

	// std::cout << g << "\n";

	// auto g2 = gdwg::graph<std::string, int>(g);

	// std::cout << g2 << "\n";

	// This is a structured binding.
	// https://en.cppreference.com/w/cpp/language/structured_binding
	// It allows you to unpack your tuple.
	// for (auto const& [from, to, weight] : g) {
	// 	std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	// }

	std::vector<int> ints{1, 2, 3, 4, 5};
	gdwg::graph<int, int> g2(ints.begin(), ints.end());
	std::cout << g2.is_node(1) << "\n";
	std::cout << g2.is_node(2) << "\n";
	std::cout << g2.is_node(3) << "\n";
	std::cout << g2.is_node(4) << "\n";
	std::cout << g2.is_node(5) << "\n";
	std::cout << g2.is_node(6) << "\n";
	std::cout << g2.insert_node(5) << "\n";
	// std::cout << g2.insert_edge(1, 2, 33);
	g2.replace_node(2, 15);
	std::cout << g2.is_node(2) << "\n";
	std::cout << g2.is_node(15) << "\n";
	// ----------------------------
	gdwg::graph<std::string, int> g3;
	{
		std::string a{"A"};
		std::string b{"B"};
		std::string c{"C"};
		std::string d{"D"};
		g3.insert_node(a);
		g3.insert_node(b);
		g3.insert_node(c);
		g3.insert_node(d);
		g3.insert_edge("A", "B", 5);
		g3.insert_edge("A", "C", 2);
		g3.insert_edge("B", "D", 22);
		g3.insert_edge("A", "D", 3);
		g3.insert_edge("B", "B", 1);
		g3.insert_edge("B", "B", 44);
	}
	g3.merge_replace_node("A", "B");
	// g3.erase_node("B");

	// g3.clear();
	// std::cout << g3.empty() << "\n";
	std::cout << g3.is_connected("B", "C") << "\n";
	std::cout << g3.is_connected("C", "B") << "\n";
	auto res = g3.nodes();
	for (const auto& r : res) {
		std::cout << r << " ";
	}
	std::cout << "\n";
	auto weights = g3.weights("B", "B");
	for (const auto& w : weights) {
		std::cout << w << " ";
	}
	std::cout << "\n";
	auto dsts = g3.connections("B");
	// for (const auto& d : dsts) {
	// 	std::cout << d << " ";
	// }
	// std::cout << "\n";
	std::cout << "==============="
	          << "\n";
	for (auto it = g3.begin(); it != g3.end(); ++it) {
		std::cout << (*it).from << " " << (*it).to << " " << (*it).weight << "\n";
	}
	std::cout << "- - - - - - - -"
	          << "\n";
	// auto it1 = g3.find("B", "B", 44);
	// auto it2 = g3.find("B", "D", 23);
	// std::cout << (*it1).from << " " << (*it1).to << " " << (*it1).weight << "\n";
	// if (it2 == g3.end()) {
	// 	std::cout << "Not Found"
	// 	          << "\n";
	// }
	// // std::cout << (*it2).from << " " << (*it2).to << " " << (*it2).weight << "\n";
	// std::cout << g3.erase_edge("B", "D", 22) << "\n";
	auto it_begin = g3.begin();
	auto it_end = g3.begin();
	++it_end;
	++it_end;
	// std::cout << (*it_end).from << " " << (*it_end).to << " " << (*it_end).weight << "\n";
	auto it4 = g3.erase_edge(it_begin, it_end);
	std::cout << (*it4).weight << "\n";
	for (auto it = g3.begin(); it != g3.end(); ++it) {
		std::cout << (*it).from << " " << (*it).to << " " << (*it).weight << "\n";
	}
	std::cout << "==============="
	          << "\n";
	auto it5 = g3.end();

	auto it6 = g3.erase_edge(it5);
	if (it6 == g3.end()) {
		std::cout << "end"
		          << "\n";
	}
	// else {
	// 	std::cout << (*it6).weight << "\n";
	// }
	for (auto it = g3.begin(); it != g3.end(); ++it) {
		std::cout << (*it).from << " " << (*it).to << " " << (*it).weight << "\n";
	}
	std::vector<std::string> s{"A", "E", "F"};
	gdwg::graph<std::string, int> g4(s.begin(), s.end());
	g4.insert_edge("A", "E", 33);
	g4.insert_edge("A", "F", 44);
	for (auto it = g4.begin(); it != g4.end(); ++it) {
		std::cout << (*it).from << " " << (*it).to << " " << (*it).weight << "\n";
	}
	auto itss = g4.begin();

	// gdwg::graph<std::string, int> g5(std::move(g4));
	gdwg::graph<std::string, int> g5 = std::move(g4);
	auto node_res = g5.nodes();

	std::cout << g4.empty() << "\n";
	for (const auto& n : node_res) {
		std::cout << n << " ";
	}
	// ++itsss;
	// --iteee;
	std::cout << "\n";
	for (auto it = itss; it != g5.end(); ++it) {
		std::cout << (*it).from << " " << (*it).to << " " << (*it).weight << "\n";
	}
	std::cout << "================"
	          << "\n";
	gdwg::graph<std::string, int> g6(s.begin(), s.end());
	gdwg::graph<std::string, int> g7(g6);
	g7.insert_node("C");
	for (const auto& n : g7.nodes()) {
		std::cout << n << " ";
	}
	std::cout << "\n";
	for (const auto& n : g6.nodes()) {
		std::cout << n << " ";
	}
	std::cout << "\n=====================\n";
	gdwg::graph<std::string, int> g8 = g7;
	for (const auto& n : g8.nodes()) {
		std::cout << n << " ";
	}
	std::cout << "\n";
	// g8.erase_node("A");
	std::cout << (g8 == g7) << "\n";
	std::cout << (g8 == g6) << "\n";
	gdwg::graph<std::string, int> g9;
	g9.insert_node("A");
	g9.insert_node("C");
	g9.insert_node("E");
	g9.insert_node("F");
	std::cout << (g8 == g9) << "\n";
	g9.insert_edge("A", "C", 33);
	std::cout << (g8 == g9) << "\n";
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
	g.insert_node(64);
	for (const auto& x : v) {
		g.insert_edge(x.from, x.to, x.weight);
	};
	std::cout << "\n";
	auto out = std::ostringstream{};
	out << g;
	std::cout << out.str();
}
