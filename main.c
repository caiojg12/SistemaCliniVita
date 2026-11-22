
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char nome[51];
    char telefone[20];
    char email[101];
    char nascimento[11];
    char cpf[12];
    char endereco[101];
} Paciente;

typedef struct {
    int id;
    char nome[51];
    char cpf[12];
    char crefito[20];
    char especialidade[51];
    char telefone[20];
    char email[101];
} Profissional;

typedef struct {
    int id;
    int idPaciente;
    int idProfissional;
    int tipoConsulta;
    char data[11];
    char inicio[6];
    char fim[6];
    char status[20];
} Agendamento;

typedef struct {
    int id;
    int idAgendamento;
    char data[11];
    char inicio[6];
    char fim[6];
    char status[20];
} Sessao;




void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void lerTexto(char texto[], int tamanho) {
    if (fgets(texto, tamanho, stdin) != NULL) {
        texto[strcspn(texto, "\n")] = '\0';

        if (strlen(texto) == (size_t)(tamanho - 1)) {
            limparEntrada();
        }
    } else {
        texto[0] = '\0';
    }
}

int cpfJaCadastrado(char cpf[]) {
    FILE *arquivo = fopen("pacientes.txt", "r");
    Paciente p;

    if (arquivo == NULL) {
        return 0;
    }

    while (fscanf(arquivo,
           "%d|%50[^|]|%19[^|]|%100[^|]|%10[^|]|%11[^|]|%100[^\n]\n",
           &p.id, p.nome, p.telefone, p.email,
           p.nascimento, p.cpf, p.endereco) == 7) {

        if (strcmp(p.cpf, cpf) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

int proximoId(void) {
    FILE *arquivo = fopen("pacientes.txt", "r");
    Paciente p;
    int maiorId = 0;

    if (arquivo == NULL) {
        return 1;
    }

    while (fscanf(arquivo,
           "%d|%50[^|]|%19[^|]|%100[^|]|%10[^|]|%11[^|]|%100[^\n]\n",
           &p.id, p.nome, p.telefone, p.email,
           p.nascimento, p.cpf, p.endereco) == 7) {

        if (p.id > maiorId) {
            maiorId = p.id;
        }
    }

    fclose(arquivo);
    return maiorId + 1;
}

void cadastrarPaciente(void) {
    Paciente p;

    printf("\n===== CADASTRO DE PACIENTES =====\n");

    printf("Nome: ");
    lerTexto(p.nome, sizeof(p.nome));

    printf("Telefone: ");
    lerTexto(p.telefone, sizeof(p.telefone));

    printf("Email: ");
    lerTexto(p.email, sizeof(p.email));

    printf("Data de nascimento (DD/MM/AAAA): ");
    lerTexto(p.nascimento, sizeof(p.nascimento));

    printf("CPF (somente numeros): ");
    lerTexto(p.cpf, sizeof(p.cpf));

    printf("Endereco: ");
    lerTexto(p.endereco, sizeof(p.endereco));

    if (strlen(p.nome) == 0 ||
        strlen(p.email) == 0 ||
        strlen(p.cpf) != 11) {

        printf("\nDados obrigatorios invalidos.\n");
        return;
    }

    if (strchr(p.nome, '|') || strchr(p.telefone, '|') ||
        strchr(p.email, '|') || strchr(p.nascimento, '|') ||
        strchr(p.cpf, '|') || strchr(p.endereco, '|')) {
        printf("\nO caractere | nao e permitido.\n");
        return;
    }

    if (cpfJaCadastrado(p.cpf)) {
        printf("\nCPF ja cadastrado!\n");
        return;
    }

    p.id = proximoId();

    FILE *arquivo = fopen("pacientes.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }

    int resultado = fprintf(arquivo,
        "%d|%s|%s|%s|%s|%s|%s\n",
        p.id, p.nome, p.telefone, p.email,
        p.nascimento, p.cpf, p.endereco);

    int fechamento = fclose(arquivo);

    if (resultado < 0 || fechamento == EOF) {
        printf("\nErro ao salvar paciente!\n");
    } else {
        printf("\nPaciente cadastrado com sucesso!\n");
        printf("ID do paciente: %d\n", p.id);
    }
}

void consultarPacientes(void) {
    FILE *arquivo = fopen("pacientes.txt", "r");
    Paciente p;
    int total = 0;

    printf("\n===== PACIENTES CADASTRADOS =====\n");

    if (arquivo == NULL) {
        printf("Nenhum arquivo de pacientes encontrado.\n");
        return;
    }

    while (fscanf(arquivo,
           "%d|%50[^|]|%19[^|]|%100[^|]|%10[^|]|%11[^|]|%100[^\n]\n",
           &p.id, p.nome, p.telefone, p.email,
           p.nascimento, p.cpf, p.endereco) == 7) {

        printf("\nID: %d\n", p.id);
        printf("Nome: %s\n", p.nome);
        printf("Telefone: %s\n", p.telefone);
        printf("Email: %s\n", p.email);
        printf("Nascimento: %s\n", p.nascimento);
        printf("CPF: %s\n", p.cpf);
        printf("Endereco: %s\n", p.endereco);
        printf("------------------------------\n");

        total++;
    }

    if (total == 0) {
        printf("Nenhum paciente cadastrado.\n");
    }

    printf("\nTotal de pacientes: %d\n", total);

    fclose(arquivo);
}

void cadastrarProfissional(void) {
    Profissional p, existente;
    FILE *arquivo;
    int maiorId = 0;

    printf("\n===== CADASTRO DE PROFISSIONAIS =====\n");

    printf("Nome: ");
    lerTexto(p.nome, sizeof(p.nome));

    printf("CPF (11 numeros): ");
    lerTexto(p.cpf, sizeof(p.cpf));

    printf("CREFITO: ");
    lerTexto(p.crefito, sizeof(p.crefito));

    printf("Especialidade: ");
    lerTexto(p.especialidade, sizeof(p.especialidade));

    printf("Telefone: ");
    lerTexto(p.telefone, sizeof(p.telefone));

    printf("Email: ");
    lerTexto(p.email, sizeof(p.email));

    if (strlen(p.nome) == 0 ||
        strlen(p.cpf) != 11 ||
        strlen(p.crefito) == 0 ||
        strlen(p.especialidade) == 0 ||
        strlen(p.email) == 0) {

        printf("\nDados obrigatorios invalidos.\n");
        return;
    }

    if (strchr(p.nome, '|') ||
        strchr(p.cpf, '|') ||
        strchr(p.crefito, '|') ||
        strchr(p.especialidade, '|') ||
        strchr(p.telefone, '|') ||
        strchr(p.email, '|')) {

        printf("\nO caractere | nao e permitido.\n");
        return;
    }

    arquivo = fopen("profissionais.txt", "r");

    if (arquivo != NULL) {
        while (fscanf(arquivo,
               "%d|%50[^|]|%11[^|]|%19[^|]|%50[^|]|%19[^|]|%100[^\n]\n",
               &existente.id,
               existente.nome,
               existente.cpf,
               existente.crefito,
               existente.especialidade,
               existente.telefone,
               existente.email) == 7) {

            if (strcmp(existente.cpf, p.cpf) == 0 ||
                strcmp(existente.crefito, p.crefito) == 0) {

                printf("\nCPF ou CREFITO ja cadastrado!\n");
                fclose(arquivo);
                return;
            }

            if (existente.id > maiorId) {
                maiorId = existente.id;
            }
        }

        fclose(arquivo);
    }

    p.id = maiorId + 1;

    arquivo = fopen("profissionais.txt", "a");

    if (arquivo == NULL) {
        printf("\nErro ao abrir arquivo de profissionais.\n");
        return;
    }

    int resultado = fprintf(arquivo,
        "%d|%s|%s|%s|%s|%s|%s\n",
        p.id, p.nome, p.cpf, p.crefito,
        p.especialidade, p.telefone, p.email);

    int fechamento = fclose(arquivo);

    if (resultado < 0 || fechamento == EOF) {
        printf("\nErro ao salvar profissional.\n");
    } else {
        printf("\nProfissional cadastrado com sucesso!\n");
        printf("ID do profissional: %d\n", p.id);
    }
}

int pacienteExiste(int id) {
    FILE *arquivo = fopen("pacientes.txt", "r");
    Paciente p;
    char linha[500];

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d|", &p.id) == 1) {
            if (p.id == id) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

int profissionalExiste(int id) {
    FILE *arquivo = fopen("profissionais.txt", "r");
    Profissional p;
    char linha[500];

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d|", &p.id) == 1) {
            if (p.id == id) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

void agendarSessao(void) {
    Agendamento a;
    FILE *arquivo;
    char linha[250];
    int maiorId = 0;
    int idLido;

    printf("\n===== AGENDAMENTO DE SESSOES =====\n");

    printf("ID do paciente: ");
    if (scanf("%d", &a.idPaciente) != 1) {
        limparEntrada();
        printf("ID invalido.\n");
        return;
    }
    limparEntrada();

    if (!pacienteExiste(a.idPaciente)) {
        printf("Paciente nao encontrado.\n");
        return;
    }

    printf("ID do profissional: ");
    if (scanf("%d", &a.idProfissional) != 1) {
        limparEntrada();
        printf("ID invalido.\n");
        return;
    }
    limparEntrada();

    if (!profissionalExiste(a.idProfissional)) {
        printf("Profissional nao encontrado.\n");
        return;
    }

    printf("\nTIPOS DE ATENDIMENTO\n");
    printf("1 - Fisioterapia Ortopedica\n");
    printf("2 - Fisioterapia Esportiva\n");
    printf("3 - Fisioterapia Neurologica\n");
    printf("Escolha: ");

    if (scanf("%d", &a.tipoConsulta) != 1) {
        limparEntrada();
        printf("Tipo invalido.\n");
        return;
    }
    limparEntrada();

    if (a.tipoConsulta < 1 || a.tipoConsulta > 3) {
        printf("Tipo de atendimento invalido.\n");
        return;
    }

    printf("Data (DD/MM/AAAA): ");
    lerTexto(a.data, sizeof(a.data));

    printf("Hora de inicio (HH:MM): ");
    lerTexto(a.inicio, sizeof(a.inicio));

    printf("Hora de termino (HH:MM): ");
    lerTexto(a.fim, sizeof(a.fim));

    arquivo = fopen("agendamentos.txt", "r");

    if (arquivo != NULL) {
        while (fgets(linha, sizeof(linha), arquivo)) {
            if (sscanf(linha, "%d|", &idLido) == 1) {
                if (idLido > maiorId) {
                    maiorId = idLido;
                }
            }
        }
        fclose(arquivo);
    }

    a.id = maiorId + 1;
    strcpy(a.status, "Agendado");

    arquivo = fopen("agendamentos.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(arquivo, "%d|%d|%d|%d|%s|%s|%s|%s\n",
        a.id, a.idPaciente, a.idProfissional,
        a.tipoConsulta, a.data, a.inicio,
        a.fim, a.status);

    fclose(arquivo);

    printf("\nAgendamento realizado com sucesso!\n");
    printf("ID do agendamento: %d\n", a.id);
}

void consultarAgendamentos(void) {
    FILE *arquivo = fopen("agendamentos.txt", "r");
    Agendamento a;
    int total = 0;

    printf("\n===== AGENDAMENTOS CADASTRADOS =====\n");

    if (arquivo == NULL) {
        printf("Nenhum agendamento encontrado.\n");
        return;
    }

    while (fscanf(arquivo,
           "%d|%d|%d|%d|%10[^|]|%5[^|]|%5[^|]|%19[^\n]\n",
           &a.id, &a.idPaciente, &a.idProfissional,
           &a.tipoConsulta, a.data, a.inicio,
           a.fim, a.status) == 8) {

        printf("\nID do agendamento: %d\n", a.id);
        printf("ID do paciente: %d\n", a.idPaciente);
        printf("ID do profissional: %d\n", a.idProfissional);
        printf("Tipo de atendimento: %d\n", a.tipoConsulta);
        printf("Data: %s\n", a.data);
        printf("Inicio: %s\n", a.inicio);
        printf("Termino: %s\n", a.fim);
        printf("Status: %s\n", a.status);
        printf("------------------------------\n");

        total++;
    }

    if (total == 0) {
        printf("Nenhum agendamento cadastrado.\n");
    }

    printf("\nTotal de agendamentos: %d\n", total);

    fclose(arquivo);
}

int agendamentoExiste(int id) {
    FILE *arquivo = fopen("agendamentos.txt", "r");
    Agendamento a;
    char linha[250];

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d|", &a.id) == 1) {
            if (a.id == id) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}

void controlarSessoes(void) {
    Sessao s;
    FILE *arquivo;
    char linha[250];
    int maiorId = 0;
    int idLido;
    int opcaoStatus;

    printf("\n===== CONTROLE DE SESSOES =====\n");

    printf("ID do agendamento: ");
    if (scanf("%d", &s.idAgendamento) != 1) {
        limparEntrada();
        printf("ID invalido.\n");
        return;
    }
    limparEntrada();

    if (!agendamentoExiste(s.idAgendamento)) {
        printf("Agendamento nao encontrado.\n");
        return;
    }

    printf("Data da sessao (DD/MM/AAAA): ");
    lerTexto(s.data, sizeof(s.data));

    printf("Hora de inicio (HH:MM): ");
    lerTexto(s.inicio, sizeof(s.inicio));

    printf("Hora de termino (HH:MM): ");
    lerTexto(s.fim, sizeof(s.fim));

    printf("\nSTATUS DA SESSAO\n");
    printf("1 - Realizada\n");
    printf("2 - Nao realizada\n");
    printf("Escolha: ");

    if (scanf("%d", &opcaoStatus) != 1) {
        limparEntrada();
        printf("Opcao invalida.\n");
        return;
    }
    limparEntrada();

    if (opcaoStatus == 1) {
        strcpy(s.status, "Realizada");
    } else if (opcaoStatus == 2) {
        strcpy(s.status, "Nao realizada");
    } else {
        printf("Opcao invalida.\n");
        return;
    }

    arquivo = fopen("sessoes.txt", "r");

    if (arquivo != NULL) {
        while (fgets(linha, sizeof(linha), arquivo)) {
            if (sscanf(linha, "%d|", &idLido) == 1) {
                if (idLido > maiorId) {
                    maiorId = idLido;
                }
            }
        }
        fclose(arquivo);
    }

    s.id = maiorId + 1;

    arquivo = fopen("sessoes.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de sessoes.\n");
        return;
    }

    fprintf(arquivo, "%d|%d|%s|%s|%s|%s\n",
        s.id, s.idAgendamento, s.data,
        s.inicio, s.fim, s.status);

    fclose(arquivo);

    printf("\nSessao registrada com sucesso!\n");
    printf("ID da sessao: %d\n", s.id);
    printf("Status: %s\n", s.status);
}





void mostrarMenu(void) {
    printf("\n===== CLINIVITA =====\n");
    printf("\n1 - Cadastrar paciente\n");
    printf("2 - Consultar pacientes\n");
    printf("3 - Cadastrar profissional\n");
    printf("4 - Agendar sessao\n");
    printf("5 - Consultar agendamentos\n");
    printf("6 - Controlar sessoes\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

int main(void) {
    int opcao;

    do {
        mostrarMenu();

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            limparEntrada();
            opcao = -1;
            continue;
        }

        limparEntrada();

        switch (opcao) {
            case 1:
                cadastrarPaciente();
                break;
            case 2:
                consultarPacientes();
                break;
            case 3:
                cadastrarProfissional();
                break;
            case 4:
                agendarSessao();
                break;
            case 5:
                consultarAgendamentos();
                break;
            case 6:
                controlarSessoes();
                break;
            case 0:
                printf("Encerrando o sistema.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
