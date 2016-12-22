/*******************************************************************\

Module: A minimalistic BDD library, following Bryant's original paper
        and Andersen's lecture notes

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SOLVERS_MINIBDD_MINIBDD_H
#define CPROVER_SOLVERS_MINIBDD_MINIBDD_H

/*! \file solvers/miniBDD/miniBDD.h
 * \brief Small BDD implementation
 *
 * \author Daniel Kroening <kroening@kroening.com>
 * \date   Mon Sep 28 00:00:00 BST 2009
*/

#include <cassert>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <stack>

class mini_bddt
{
public:
  inline mini_bddt();
  inline mini_bddt(const mini_bddt &x);
  inline ~mini_bddt();

  // Boolean operators on BDDs
  mini_bddt operator!() const;
  mini_bddt operator^(const mini_bddt &) const;
  mini_bddt operator==(const mini_bddt &) const;
  mini_bddt operator&(const mini_bddt &) const;
  mini_bddt operator|(const mini_bddt &) const;

  // copy operator
  inline mini_bddt &operator=(const mini_bddt &);

  inline bool is_constant() const;
  inline bool is_true() const;
  inline bool is_false() const;

  inline unsigned var() const;
  inline const mini_bddt &low() const;
  inline const mini_bddt &high() const;
  inline unsigned node_number() const;
  inline void clear();

  bool is_initialized() const { return node!=0; }

  // internal
  explicit inline mini_bddt(class mini_bdd_nodet *_node);
  class mini_bdd_nodet *node;
};

class mini_bdd_nodet
{
public:
  class mini_bdd_mgrt *mgr;
  unsigned var, node_number, reference_counter;
  mini_bddt low, high;

  inline mini_bdd_nodet(
    class mini_bdd_mgrt *_mgr,
    unsigned _var, unsigned _node_number,
    const mini_bddt &_low, const mini_bddt &_high);

  inline void add_reference();
  void remove_reference();
};

class mini_bdd_mgrt
{
public:
  mini_bdd_mgrt();
  ~mini_bdd_mgrt();

  mini_bddt Var(const std::string &label);

  void DumpDot(std::ostream &out, bool supress_zero=false) const;
  void DumpTikZ(
    std::ostream &out,
    bool supress_zero=false,
    bool node_numbers=true) const;
  void DumpTable(std::ostream &out) const;

  inline const mini_bddt &True() const;
  inline const mini_bddt &False() const;

  friend class mini_bddt;
  friend class mini_bdd_nodet;

  // create a node (consulting the reverse-map)
  mini_bddt mk(unsigned var, const mini_bddt &low, const mini_bddt &high);

  inline std::size_t number_of_nodes();

  struct var_table_entryt
  {
    std::string label;
    inline var_table_entryt(const std::string &_label);
  };

  typedef std::vector<var_table_entryt> var_tablet;
  var_tablet var_table;

protected:
  typedef std::list<mini_bdd_nodet> nodest;
  nodest nodes;
  mini_bddt true_bdd, false_bdd;

  // this is our reverse-map for nodes
  struct reverse_keyt
  {
    unsigned var, low, high;
    inline reverse_keyt(
      unsigned _var, const mini_bddt &_low, const mini_bddt &_high);

    bool operator<(const reverse_keyt &other) const;
  };

  typedef std::map<reverse_keyt, mini_bdd_nodet *> reverse_mapt;
  reverse_mapt reverse_map;

  typedef std::stack<mini_bdd_nodet *> freet;
  freet free;
};

mini_bddt restrict(const mini_bddt &u, unsigned var, const bool value);
mini_bddt exists(const mini_bddt &u, unsigned var);
mini_bddt substitute(
  const mini_bddt &where,
  unsigned var,
  const mini_bddt &by_what);
std::string cubes(const mini_bddt &u);
bool OneSat(const mini_bddt &v, std::map<unsigned, bool> &assignment);

// inline functions
#include "miniBDD.inc"

#endif // CPROVER_SOLVERS_MINIBDD_MINIBDD_H
