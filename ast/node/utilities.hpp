

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

static string ToCCode(const shared_ptr<Node> &nd) { return nd->toCCode(); }

static vector<string> ToCCode(const vector<shared_ptr<Node>> &nds) {
  vector<string> trgt;
  trgt.resize(nds.size());
  transform(nds.begin(), nds.end(), trgt.begin(),
            [](shared_ptr<Node> nd) { return ToCCode(nd); });
  return trgt;
}
template <typename T>
static string convertToString(const T &nd) { 
  ostringstream o;
  o << nd;
  return o.str();
}

static string ToString(const shared_ptr<Node> &nd) { return nd->toString(); }
static vector<string> ToString(const vector<shared_ptr<Node>> &nds) {
  vector<string> trgt;
  trgt.resize(nds.size());
  transform(nds.begin(), nds.end(), trgt.begin(),
            [](shared_ptr<Node> nd) { return ToString(nd); });
  return trgt;
}

static void StringJoin(ostringstream &o, const vector<string> &lst) {
  for (auto s : lst) {
    o << s;
  }
}

static string StringJoin(const vector<string> &lst) {
  ostringstream o;
  StringJoin(o, lst);
  return o.str();
}

template <typename T>
static vector<T> Riffle(const vector<T> &lst, const T &sep) {
  auto len = lst.size();
  vector<T> res;
  for (auto s : lst) {
    res.push_back(s);
    if (len-- > 1) {
      res.push_back(sep);
    }
  }
  return res;
}

static void RiffleJoin(ostringstream &o, const vector<string> &lst,
                       const string &sep) {
  auto len = lst.size();
  for (auto s : lst) {
    o << s;
    if (len-- > 1) {
      o << sep;
    }
  }
}

static string RiffleJoin(const vector<string> &lst, const string &sep) {
  ostringstream o;
  RiffleJoin(o, lst, sep);
  return o.str();
}

#endif /* __UTILITIES_H__ */
