#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct {
	unsigned income_cap;
	double tax_rate;
	double overall_tax;
	double overall_tax_rate;
}
tax_bracket_t;

typedef enum {
	INPUT_INCOME,
	INPUT_OVERALL_TAX,
	INPUT_OVERALL_TAX_RATE,
	INPUT_QUIT
}
input_type_t;

void output_from_income(void);
void output_from_overall_tax(void);
void output_from_overall_tax_rate(void);

size_t tax_brackets_n;
tax_bracket_t *tax_brackets;

int main(void) {
	input_type_t input_type;

	/* Read tax brackets on standard input, check <income cap> / <tax rate> are properly sorted */
	/* Only <tax rate> is read for last tax bracket, <income cap> is set to UINT_MAX */
	/* Cumulative <overall tax> / <overall tax rate> are computed for each bracket */
	if (scanf("%zu", &tax_brackets_n) != 1 || tax_brackets_n < 1) {
		fprintf(stderr, "Invalid number of tax brackets\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	tax_brackets = malloc(sizeof(tax_bracket_t)*tax_brackets_n);
	if (!tax_brackets) {
		fprintf(stderr, "Could not allocate memory for tax brackets\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	if (tax_brackets_n > 1) {
		size_t i;
		if (scanf("%u%lf", &tax_brackets[0].income_cap, &tax_brackets[0].tax_rate) != 2 || tax_brackets[0].tax_rate < 0 || tax_brackets[0].tax_rate > 1) {
			fprintf(stderr, "Invalid tax bracket\n");
			fflush(stderr);
			free(tax_brackets);
			return EXIT_FAILURE;
		}
		tax_brackets[0].overall_tax = tax_brackets[0].income_cap*tax_brackets[0].tax_rate;
		tax_brackets[0].overall_tax_rate = tax_brackets[0].overall_tax/tax_brackets[0].income_cap;
		for (i = 1; i < tax_brackets_n-1; i++) {
			if (scanf("%u%lf", &tax_brackets[i].income_cap, &tax_brackets[i].tax_rate) != 2 || tax_brackets[i].income_cap <= tax_brackets[i-1].income_cap || tax_brackets[i].tax_rate <= tax_brackets[i-1].tax_rate || tax_brackets[i].tax_rate > 1) {
				fprintf(stderr, "Invalid tax bracket\n");
				fflush(stderr);
				free(tax_brackets);
				return EXIT_FAILURE;
			}
			tax_brackets[i].overall_tax = tax_brackets[i-1].overall_tax+(tax_brackets[i].income_cap-tax_brackets[i-1].income_cap)*tax_brackets[i].tax_rate;
			tax_brackets[i].overall_tax_rate = tax_brackets[i].overall_tax/tax_brackets[i].income_cap;
		}
		tax_brackets[i].income_cap = UINT_MAX;
		if (tax_brackets[i].income_cap == tax_brackets[i-1].income_cap || scanf("%lf", &tax_brackets[i].tax_rate) != 1 || tax_brackets[i].tax_rate <= tax_brackets[i-1].tax_rate || tax_brackets[i].tax_rate > 1) {
			fprintf(stderr, "Invalid tax bracket\n");
			fflush(stderr);
			free(tax_brackets);
			return EXIT_FAILURE;
		}
		tax_brackets[i].overall_tax = tax_brackets[i-1].overall_tax+(tax_brackets[i].income_cap-tax_brackets[i-1].income_cap)*tax_brackets[i].tax_rate;
		tax_brackets[i].overall_tax_rate = tax_brackets[i].overall_tax/tax_brackets[i].income_cap;
	}
	else {
		tax_brackets[0].income_cap = UINT_MAX;
		if (scanf("%lf", &tax_brackets[0].tax_rate) != 1 || tax_brackets[0].tax_rate < 0 || tax_brackets[0].tax_rate > 1) {
			fprintf(stderr, "Invalid tax bracket\n");
			fflush(stderr);
			free(tax_brackets);
			return EXIT_FAILURE;
		}
		tax_brackets[0].overall_tax = tax_brackets[0].income_cap*tax_brackets[0].tax_rate;
		tax_brackets[0].overall_tax_rate = tax_brackets[0].overall_tax/tax_brackets[0].income_cap;
	}

	/* Read requests on standard input */
	/* 0 <income> computes <overall tax> <overall tax rate> */
	/* 1 <overall tax> computes <income> <overall tax rate> */
	/* 2 <overall tax rate> computes <income> <overall tax> */
	/* 3 quit */
	printf("0 <income>\n1 <overall tax>\n2 <overall tax rate>\n3 quit\n");
	fflush(stdout);
	do {
		if (scanf("%u", &input_type) == 1) {
			switch (input_type) {
				case INPUT_INCOME:
				output_from_income();
				break;
				case INPUT_OVERALL_TAX:
				output_from_overall_tax();
				break;
				case INPUT_OVERALL_TAX_RATE:
				output_from_overall_tax_rate();
				break;
				case INPUT_QUIT:
				puts("Bye!");
				fflush(stdout);
				break;
				default:
				fprintf(stderr, "Invalid input type\n");
				fflush(stderr);
			}
		}
		else {
			fprintf(stderr, "Invalid input type\n");
			fflush(stderr);
		}
	}
	while (input_type != INPUT_QUIT);
	free(tax_brackets);
	return EXIT_SUCCESS;
}

/* Input <income> Output <overall tax> <overall tax rate> */
void output_from_income(void) {
	unsigned income;
	size_t i;
	if (scanf("%u", &income) != 1) {
		fprintf(stderr, "Invalid income\n");
		fflush(stderr);
		return;
	}
	for (i = 0; i < tax_brackets_n && tax_brackets[i].income_cap < income; i++);
	if (i == 0) {
		printf("Overall tax %.0lf\nOverall tax rate %.2lf\n", floor(income*tax_brackets[0].tax_rate), tax_brackets[0].tax_rate);
	}
	else {
		double overall_tax = tax_brackets[i-1].overall_tax+(income-tax_brackets[i-1].income_cap)*tax_brackets[i].tax_rate;
		printf("Overall tax %.0lf\nOverall tax rate %.2lf\n", floor(overall_tax), overall_tax/income);
	}
	fflush(stdout);
}

/* Input <overall tax> Output <income> <overall tax rate> */
void output_from_overall_tax(void) {
	unsigned income;
	double overall_tax;
	size_t i;
	if (scanf("%lf", &overall_tax) != 1 || overall_tax < tax_brackets[0].overall_tax || overall_tax > tax_brackets[tax_brackets_n-1].overall_tax) {
		fprintf(stderr, "Invalid overall tax\n");
		fflush(stderr);
		return;
	}
	for (i = 0; i < tax_brackets_n && tax_brackets[i].overall_tax < overall_tax; i++);
	if (i == 0) {
		if (tax_brackets[0].tax_rate > 0) {
			income = (unsigned)floor(overall_tax/tax_brackets[0].tax_rate);
		}
		else {
			income = 0;
		}
	}
	else {
		income = tax_brackets[i-1].income_cap+(unsigned)floor((overall_tax-tax_brackets[i-1].overall_tax)/tax_brackets[i].tax_rate);
	}
	if (income > 0) {
		printf("Income %u\nOverall tax rate %.2lf\n", income, overall_tax/income);
	}
	else {
		printf("Income 0\nOverall tax rate 0.00\n");
	}
	fflush(stdout);
}

/* Input <overall tax rate> Output <income> <overall tax> */
void output_from_overall_tax_rate(void) {
	unsigned income;
	double overall_tax_rate;
	size_t i;
	if (scanf("%lf", &overall_tax_rate) != 1 || overall_tax_rate < tax_brackets[0].overall_tax_rate || overall_tax_rate > tax_brackets[tax_brackets_n-1].overall_tax_rate) {
		fprintf(stderr, "Invalid overall tax rate\n");
		fflush(stderr);
		return;
	}
	for (i = 0; i < tax_brackets_n && tax_brackets[i].overall_tax_rate < overall_tax_rate; i++);
	if (i == 0) {
		if (overall_tax_rate > 0) {
			income = (unsigned)floor(tax_brackets[0].overall_tax/overall_tax_rate);
		}
		else {
			income = 0;
		}
	}
	else {
		/* <income> lies between <income_cap>[i-1] and <income_cap>[i] */
		/* The below equation is used to compute <income> from <overall tax rate> */
		/* <income> * <overall tax rate> = <overall_tax>[i-1] + (<income> - <income_cap>[i-1]) * <tax_rate>[i] */
		/* <income> is on both sides of the equation, same equation with <income> on the left side is */
		/* <income> = (<overall_tax>[i-1] - <income_cap>[i-1] * <tax_rate>[i]) / (<overall tax rate> - <tax_rate>[i]) */
		income = (unsigned)floor((tax_brackets[i-1].overall_tax-tax_brackets[i-1].income_cap*tax_brackets[i].tax_rate)/(overall_tax_rate-tax_brackets[i].tax_rate));
	}
	printf("Income %u\nOverall tax %.0lf\n", income, floor(income*overall_tax_rate));
	fflush(stdout);
}
