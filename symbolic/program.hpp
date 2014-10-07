

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

class ProgramNode : public CompoundNode {
public:
  ProgramNode() : CompoundNode() {}
  ~ProgramNode() {}
  string getHead() { return head_; }
  bool isProgram() const { return true; }

private:
  string head_ = "Program";
};

#endif /* __PROGRAM_H__ */
