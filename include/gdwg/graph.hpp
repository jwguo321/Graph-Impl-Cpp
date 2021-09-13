#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

// TODO: Make this graph generic
//       ... this won't just compile
//       straight away
#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <vector>
namespace gdwg {

	template<typename N, typename E>
	class graph {
	public:
		struct value_type {
			N from;
			N to;
			E weight;
			value_type(N src, N t, E w)
			: from{src}
			, to{t}
			, weight{w} {}
		};
		class iterator;
		/*
		   2.2	Constructors
		*/

		// default constructor
		graph() noexcept = default;
		// initializer list construtor
		graph(std::initializer_list<N> il) noexcept
		: graph(il.begin(), il.end()){};
		// iterator construtor
		template<typename InputIt>
		graph(InputIt first, InputIt last) noexcept {
			for (auto it = first; it != last; ++it) {
				insert_node(*it);
			}
		}
		graph(graph&& other) noexcept
		: nodes_{std::move(other.nodes_)}
		, edges_{std::move(other.edges_)} {
			// other.clear();
		}
		auto operator=(graph&& other) noexcept -> graph& {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
			return *this;
		}
		// copy constructor
		graph(graph const& other) noexcept = default;
		// copy assignment
		auto operator=(graph const& other) noexcept -> graph& {
			if (this != &other) {
				auto node_size = other.nodes_.size();
				auto edge_size = other.edges_.size();
				std::copy(&other.edges_[0], &other.edges_ + edge_size, &edges_[0]);
				std::copy(&other.nodes_[0], &other.nodes_ + node_size, &nodes_[0]);
			}
			return *this;
		}
		/*
		   Section 2.3	Modifiers
		*/

		auto insert_node(N const& value) noexcept -> bool {
			if (nodes_.find(value) == nodes_.end()) {
				return nodes_.insert(std::make_shared<node>(value)).second;
			}
			return false;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
			auto src_p = *nodes_.find(src);
			auto dst_p = *nodes_.find(dst);
			return edges_.insert(std::make_shared<edge>(src_p, dst_p, weight)).second;
		}
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			if (is_node(new_data)) {
				return false;
			}
			auto node = *nodes_.find(old_data);
			nodes_.erase(node);
			node->change_value(new_data);
			nodes_.insert(node);
			return true;
		}
		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
			auto node_to_be_replace = *nodes_.find(new_data);
			auto node_to_be_erase = *nodes_.find(old_data);
			nodes_.erase(node_to_be_erase);
			std::shared_ptr<node> p;
			std::vector<std::shared_ptr<edge>> edge_to_be_erase;
			std::vector<std::shared_ptr<edge>> edge_to_be_insert;
			for (auto e : edges_) {
				value_type v(e->src(), e->dst(), e->weight());
				int change = 0;
				auto src_node = e->src_ptr();
				auto dst_node = e->dst_ptr();
				if (v.from == old_data) {
					p = e->src_ptr();
					v.from = new_data;
					change = 1;
					src_node = node_to_be_replace;
				}
				if (v.to == old_data) {
					p = e->dst_ptr();
					v.to = new_data;
					change = 1;
					dst_node = node_to_be_replace;
				}
				if (change == 1) {
					edge_to_be_insert.push_back(std::make_shared<edge>(src_node, dst_node, e->weight()));
					edge_to_be_erase.push_back(e);
				}
			}
			for (auto e : edge_to_be_erase) {
				edges_.erase(e);
			}
			for (auto e : edge_to_be_insert) {
				edges_.insert(e);
			}
			// p.reset();
		}
		auto erase_node(N const& value) noexcept -> bool {
			std::vector<std::shared_ptr<node>> node_list;
			std::vector<std::shared_ptr<edge>> edge_list;
			size_t number = 0;
			if (!is_node(value)) {
				return false;
			}
			for (auto n : nodes_) {
				if (n->value() == value) {
					node_list.push_back(n);
				}
			}
			for (auto e : edges_) {
				if (e->src() == value || e->dst() == value) {
					edge_list.push_back(e);
				}
			}
			for (auto n : node_list) {
				number = nodes_.erase(n);
				// n.reset();
			}
			for (auto e : edge_list) {
				number = edges_.erase(e);
				// e.reset();
			}
			return number == 1;
		}
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			value_type v(src, dst, weight);
			auto it = this->edges_.find(v);
			if (it != edges_.end()) {
				auto number = edges_.erase(*it);
				return number == 1;
			}
			return false;
		}
		auto erase_edge(iterator i) noexcept -> iterator;
		auto erase_edge(iterator i, iterator s) noexcept -> iterator;
		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
		}
		/*
		   Section 2.4 Accessors
		*/
		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool {
			return (nodes_.find(value) != nodes_.end());
		}
		[[nodiscard]] auto empty() const noexcept -> bool {
			return nodes_.size() == 0;
		}
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			for (auto e : edges_) {
				if (e->src() == src && e->dst() == dst) {
					return true;
				}
			}
			return false;
		}
		[[nodiscard]] auto nodes() const noexcept -> std::vector<N> {
			std::vector<N> res;
			for (auto n : nodes_) {
				res.push_back(n->value());
			}
			return res;
		}
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			std::vector<E> res;
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			for (auto e : edges_) {
				if (e->src() == src && e->dst() == dst) {
					res.push_back(e->weight());
				}
			}
			return res;
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			std::vector<N> res;
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			for (auto e : edges_) {
				if (e->src() == src) {
					if (res.empty()) {
						res.push_back(e->dst());
					}
					else {
						if (e->dst() != res.back()) {
							res.push_back(e->dst());
						}
					}
				}
			}
			return res;
		}
		/*
		   Section 2.6 Comparisons
		*/
		[[nodiscard]] auto operator==(graph const& other) const noexcept -> bool {
			if (this->edges_.size() != other.edges_.size() || this->nodes_.size() != other.nodes_.size())
			{
				return false;
			}
			auto other_it = other.nodes_.begin();
			for (auto it = this->nodes_.begin(); it != this->nodes_.end(); ++it) {
				if ((*it)->value() != (*other_it)->value()) {
					return false;
				}
				++other_it;
			}
			auto other_it2 = other.edges_.begin();
			for (auto it = this->edges_.begin(); it != this->edges_.end(); ++it) {
				if ((*it)->src() != (*other_it2)->src() || (*it)->dst() != (*other_it2)->dst()
				    || (*it)->weight() != (*other_it2)->weight())
				{
					return false;
				}
				++other_it;
			}
			return true;
		}

		/*
		   Section 2.7 Extractor
		*/
		friend auto operator<<(std::ostream& os, graph const& g) noexcept -> std::ostream& {
			if (g.empty()) {
				os << "()";
				return os;
			}
			auto nodes = g.nodes();
			auto left_p = 0;
			// os << "(";
			for (auto n : nodes) {
				auto conn = g.connections(n);
				if (conn.size() == 0) {
					os << n << " (\n)\n";
				}
				else {
					auto start = 0;
					for (auto it = g.edges_.begin(); it != g.edges_.end(); it++) {
						if ((*it)->src() == n) {
							if (start == 0) {
								os << n << " (\n";
								left_p = 1;
								os << "  " << (*it)->dst() << " | " << (*it)->weight() << "\n";
								start = 1;
							}
							else {
								os << "  " << (*it)->dst() << " | " << (*it)->weight() << "\n";
							}
						}
						else if ((*it)->src() > n) {
							left_p = 0;
							os << ")\n";
							break;
						}
					}
				}
				if (left_p == 1) {
					os << ")\n";
					left_p = 0;
				}
			}
			// os << ")";
			return os;
		}

	private:
		class node {
		public:
			node() {
				value_ = N{};
			}
			explicit node(N const& value) {
				value_ = value;
			}
			auto value() {
				return value_;
			}
			auto change_value(N const& value) {
				value_ = value;
			}

		private:
			N value_{};
		};

		// edge class
		class edge {
		public:
			edge() = default;
			edge(std::shared_ptr<node> src, std::shared_ptr<node> dst, E w) {
				src_ = src;
				dst_ = dst;
				weight_ = w;
			}
			auto src() {
				return src_->value();
			}
			auto dst() {
				return dst_->value();
			}
			auto weight() {
				return weight_;
			}
			auto src_ptr() {
				return src_;
			}
			auto dst_ptr() {
				return dst_;
			}
			auto replace_ptr(char const* s, std::shared_ptr<node> const& p) {
				const char* s2 = "src";
				if (s == s2) {
					src_ = p;
				}
				else {
					dst_ = p;
				}
			}

		private:
			std::shared_ptr<node> src_;
			std::shared_ptr<node> dst_;
			E weight_;
		};
		// use std::set to store node list and edge list
		struct node_cmp {
			using is_transparent = std::true_type;
			auto operator()(std::shared_ptr<node> const& n1, std::shared_ptr<node> const& n2) const
			   -> bool {
				return n1->value() < n2->value();
			}
			auto operator()(std::shared_ptr<node> const& n1, N const& n2) const -> bool {
				return n1->value() < n2;
			}
			auto operator()(N const& n1, std::shared_ptr<node> const& n2) const -> bool {
				return n1 < n2->value();
			}
		};
		struct edge_cmp {
			using is_transparent = std::true_type;
			auto operator()(std::shared_ptr<edge> const& e1, std::shared_ptr<edge> const& e2) const
			   -> bool {
				if (e1->src() != e2->src()) {
					return e1->src() < e2->src();
				}
				if (e1->dst() != e2->dst()) {
					return e1->dst() < e2->dst();
				}
				return e1->weight() < e2->weight();
			}
			auto operator()(std::shared_ptr<edge> const& e1, value_type const& e2) const -> bool {
				if (e1->src() != e2.from) {
					return e1->src() < e2.from;
				}
				if (e1->dst() != e2.to) {
					return e1->dst() < e2.to;
				}
				return e1->weight() < e2.weight;
			}
			auto operator()(value_type const& e1, std::shared_ptr<edge> const& e2) const -> bool {
				if (e1.from != e2->src()) {
					return e1.from < e2->src();
				}
				if (e1.to != e2->dst()) {
					return e1.to < e2->dst();
				}
				return e1.weight < e2->weight();
			}
		};
		std::set<std::shared_ptr<node>, node_cmp> nodes_;
		std::set<std::shared_ptr<edge>, edge_cmp> edges_;

	public:
		/*
		   Section 2.4 Accessors(Continued)
		*/
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const noexcept
		   -> iterator {
			value_type v(src, dst, weight);
			auto it = edges_.find(v);
			auto i = iterator(it);

			return i;
		}
		/*
		   Section 2.5 Iterator access
		*/
		[[nodiscard]] auto begin() const noexcept -> iterator {
			auto it = iterator(this->edges_.begin());
			return it;
		}
		[[nodiscard]] auto end() const noexcept -> iterator {
			auto it = iterator(this->edges_.end());
			return it;
		}
	};
	/*
	   Section 2.8 Iterator
	*/
	template<typename N, typename E>
	class graph<N, E>::iterator {
	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// Iterator constructor
		iterator() = default;
		explicit iterator(typename std::set<std::shared_ptr<edge>>::const_iterator it)
		: iterator_{it} {}

		// Iterator source
		auto operator*() -> reference {
			value_type v((*iterator_)->src(), (*iterator_)->dst(), (*iterator_)->weight());
			return v;
		}

		// Iterator traversal
		auto operator++() -> iterator& {
			++iterator_;
			return *this;
		}
		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}
		auto operator--() -> iterator& {
			--iterator_;
			return *this;
		}
		auto operator--(int) -> iterator {
			auto temp = *this;
			--*this;
			return temp;
		}

		// Iterator comparison
		auto operator==(iterator const& other) const -> bool {
			return this->iterator_ == other.iterator_;
		}

	private:
		typename std::set<std::shared_ptr<edge>>::iterator iterator_;
	};
	/*
	Section 2.3 Modifiers(Continued)
	*/
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i) noexcept -> iterator {
		if (i == iterator(edges_.end())) {
			return i;
		}
		value_type v((*i).from, (*i).to, (*i).weight);
		auto it = edges_.find(v);
		auto it2 = edges_.erase(it);
		return iterator(it2);
	}
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) noexcept -> iterator {
		if (i == end() || i == s) {
			return i;
		}
		value_type v1((*i).from, (*i).to, (*i).weight);
		value_type v2((*s).from, (*s).to, (*s).weight);
		auto it1 = edges_.find(v1);
		auto it2 = edges_.find(v2);
		auto it = edges_.erase(it1, it2);
		return iterator(it);
	}
} // namespace gdwg

#endif // GDWG_GRAPH_HPP
