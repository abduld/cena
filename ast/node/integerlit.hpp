
#ifndef __INTEGER_LIT_H__
#define __INTEGER_LIT_H__

class IntegerNode : public virtual AtomNode<int64_t>,
                    public NodeAcceptor<IntegerNode> {
public:
  IntegerNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw) {}
  IntegerNode(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const override  { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("IntegerLiteral");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = getConstant();
    return obj;
  }
private:
  string head_ = "Integer";
};

class Integer8Node : public virtual AtomNode<int8_t>,
                    public NodeAcceptor<Integer8Node> {
public:
  Integer8Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int8_t>(row, col, endrow, endcol, raw) {}
  Integer8Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int8_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const override  { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("Integer8Literal");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = getConstant();
    return obj;
  }
private:
  string head_ = "Integer8";
};

class Integer16Node : public virtual AtomNode<int16_t>,
                    public NodeAcceptor<Integer16Node> {
public:
  Integer16Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int16_t>(row, col, endrow, endcol, raw) {}
  Integer16Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int16_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const  override  { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("Integer16Literal");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = getConstant();
    return obj;
  }
private:
  string head_ = "Integer16";
};

class Integer32Node : public virtual AtomNode<int32_t>,
                    public NodeAcceptor<Integer32Node> {
public:
  Integer32Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int32_t>(row, col, endrow, endcol, raw) {}
  Integer32Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int32_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const override  { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("Integer32Literal");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = getConstant();
    return obj;
  }
private:
  string head_ = "Integer32";
};
class Integer64Node : public virtual AtomNode<int64_t>,
                    public NodeAcceptor<Integer64Node> {
public:
  Integer64Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw) {}
  Integer64Node(const int &row, const int &col, const int &endrow,
              const int &endcol, const string &raw, const int64_t &v)
      : AtomNode<int64_t>(row, col, endrow, endcol, raw, v) {}
  string getHead() const override { return head_; }
  void traverse(ASTVisitor *visitor) override { accept(visitor); }

  virtual Json toEsprima_() override {
    Json::object obj;
    obj["type"] = Json("Integer64Literal");
    obj["loc"] = getLocation();
    obj["raw"] = raw_;
    obj["cform"] = toCCode();
    obj["value"] = getConstant();
    return obj;
  }
private:
  string head_ = "Integer64";
};
#endif /* __INTEGER_LIT_H__ */
