#include "9cc.h"

static int count(void) {
  static int i = 1;
  return i++;
}

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR) {
    error("代入の左辺値が変数ではありません");
  }

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node) {
  if (!node) return;
  switch (node->kind) {
    case ND_NUM:
      printf("  push %d\n", node->val);
      return;
    case ND_LVAR:
      gen_lval(node);
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;
    case ND_IF: {
      int c = count();
      gen(node->cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lelse%03d\n", c);
      gen(node->then);
      printf("  jmp .Lend%03d\n", c);
      printf(".Lelse%03d:\n", c);
      if (node->els) {
        gen(node->els);
      }
      printf(".Lend%03d:\n", c);
      return;
    }
    case ND_WHILE: {
      int c = count();
      printf(".Lbegin%03d:\n", c);
      gen(node->cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%03d\n", c);
      gen(node->then);
      printf("  jmp .Lbegin%03d\n", c);
      printf(".Lend%03d:\n", c);
      return;
    }
    case ND_FOR: {
      int c = count();
      gen(node->init);
      printf(".Lbegin%03d:\n", c);
      gen(node->cond);
      if (!node->cond) {
        printf("  push 1\n");
      }
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .Lend%03d\n", c);
      gen(node->then);
      gen(node->inc);
      printf("  jmp .Lbegin%03d\n", c);
      printf(".Lend%03d:\n", c);
      return;
    }
    case ND_BLOCK:
      for (int i = 0; node->block[i]; i++) {
        gen(node->block[i]);
      }
      return;
    case ND_RETURN:
      gen(node->lhs);
      printf("  pop rax\n");
      printf("  mov rsp, rbp\n");
      printf("  pop rbp\n");
      printf("  ret\n");
      return;
    case ND_ASSIGN:
      gen_lval(node->lhs);
      gen(node->rhs);

      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  mov [rax], rdi\n");
      printf("  push rdi\n");
      return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NE:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  printf("  push rax\n");
}
