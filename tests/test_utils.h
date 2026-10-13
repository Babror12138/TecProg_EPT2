#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <stdio.h>
#include <math.h>

static int tests_run = 0;
static int tests_failed = 0;

static inline void checkTrue(int ok, const char *expr, const char *file, int line) {
    tests_run++;
    if (!ok)
    {
        tests_failed++;
        printf("  FALHA: %s (%s:%d)\n", expr, file, line);
    }
}

static inline void checkNear(double obtido, double esperado, double eps, const char *expr_obtido, const char *expr_esperado, const char *file, int line) {
    tests_run++;
    if (fabs(obtido - esperado) > eps) {
        tests_failed++;
        printf("  FALHOU: %s != %s (obtido %g, esperado %g) (%s:%d)\n", expr_obtido, expr_esperado, obtido, esperado, file, line);
    }
}

/**
 * @brief Verifica se uma condição é verdadeira e registra falha caso contrário
 * @param cond expressão que deve ser verdadeira
 */
#define ASSERT_TRUE(cond) checkTrue(!!(cond), #cond, __FILE__, __LINE__)

/**
 * @brief Verifica se dois doubles são iguais (com uma tolerância)
 * @param obtido valor obtido no programa
 * @param esperado valor esperado no teste
 * @param eps tolerância máxima aceita
 */
#define ASSERT_NEAR(obtido, esperado, eps) checkNear(obtido, esperado, eps, #obtido, #esperado, __FILE__, __LINE__)

/**
 * @brief Imprime o resumo do módulo que testado e retorna o código de saída do teste
 * @param modulo nome do módulo em questão
 * @return 0 se todos testes passaram e 1 se houve alguma falha
 */
static inline int testReport(const char* modulo) {
	printf("[%s] %d/%d testes passaram -> %s\n", modulo, tests_run - tests_failed, tests_run, tests_failed == 0 ? "OK" : "FALHA");
	return tests_failed == 0 ? 0 : 1;
}

#endif