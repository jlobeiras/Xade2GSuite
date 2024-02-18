#include <time.h>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <time.h>

// Global variable with the name of the executable
const char* exeName = 0;
static int dotLess = 0;
static FILE* glbFile = 0;

// Inicialización parcial, resto en main
std::map<char, char> conv = {
	{ 'á', 'a' }, { 'à', 'a' }, { 'â', 'a' }, { 'ä', 'a' }, { 'Á', 'a' }, { 'À', 'a' }, { 'Â', 'a' }, { 'Ä', 'a' },

	{ 'é', 'e' }, { 'è', 'e' }, { 'ê', 'e' }, { 'ë', 'e' }, { 'É', 'e' }, { 'È', 'e' }, { 'Ê', 'e' }, { 'Ë', 'e' },

	{ 'í', 'i' }, { 'ì', 'i' }, { 'î', 'i' }, { 'ï', 'i' }, { 'Í', 'i' }, { 'Ì', 'i' }, { 'Î', 'i' }, { 'Ï', 'i' },

	{ 'ó', 'o' }, { 'ò', 'o' }, { 'ô', 'o' }, { 'ö', 'o' }, { 'Ó', 'o' }, { 'Ò', 'o' }, { 'Ô', 'o' }, { 'Ö', 'o' },

	{ 'ú', 'u' }, { 'ù', 'u' }, { 'û', 'u' }, { 'ü', 'u' }, { 'Ú', 'u' }, { 'Ù', 'u' }, { 'Û', 'u' }, { 'Ü', 'u' },

	{ 'ñ', 'n' }, { 'Ñ', 'n' },

	{ 'ç', 'c' }, { 'Ç', 'c' },

	{ '.', '.'}, { '@', '@' }
};

int SearchAndReplace(std::string &source, const std::string &search,
		const std::string &replace, int maxReplace = -1) {
	// Number of replacements
	int cont = 0;

	// Search for first ocurrence
	size_t pos = source.find(search);

	// While not end of source string
	while (pos != std::string::npos && cont != maxReplace) {

		// Replace substring
		source.replace(pos, search.length(), replace);
		pos += replace.length();

		// Find next ocurrence
		pos = source.find(search, pos);

		// Do not exceed the maximum number of replacements
		cont++;
	}

	return cont;
}

std::string getYear() {
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);
	// int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1;
	int year = aTime->tm_year + 1900;
	if(month < 8) year--;
	return std::to_string((year - 2000) * 100 + (year - 2000 + 1));
}

std::string TextClean(const std::string &source) {
	std::string temp;
	for(const char& character : source)
		if(conv.find(character) != conv.end())
			temp.push_back(conv[character]);
	return temp;
}

class Estudiante {

public:
	Estudiante() { }

	void setNombre(const std::string& nome) { _nome = nome; }
	void setApelido1(const std::string& apelido1) { _apelido1 = apelido1; }
	void setApelido2(const std::string& apelido2) { _apelido2 = apelido2; }
	void setCurso(const std::string& curso) { _curso = curso; }
	void setGrupo(const std::string& grupo) { _grupo = grupo; }

	const std::string& getNombre() const { return _nome; }
	const std::string& getApelido1() const { return _apelido1; }
	const std::string& getApelido2() const { return _apelido2; }
	const std::string& getCurso() const { return _curso; }
	const std::string& getGrupo() const { return _grupo; }

	const std::string& set(const std::string& field, const std::string& value) {
		if(field == "NOME" || field == "Nome alumno") return _nome = value;
		if(field == "APELIDO1" || field == "Primeiro apelido") return _apelido1 = value;
		if(field == "APELIDO2" || field == "Segundo apelido") return _apelido2 = value;
		if(field == "GRUPO" || field == "Grupo") return _grupo = value;
		if(field == "CURSO" || field == "Curso") {
			_curso = value;
			SearchAndReplace(_curso, "1º ", "1_");
			SearchAndReplace(_curso, "2º ", "2_");
			SearchAndReplace(_curso, "3º ", "3_");
			SearchAndReplace(_curso, "4º ", "4_");
			SearchAndReplace(_curso, "Bacharelato", "BACH");
			SearchAndReplace(_curso, "Administración e finanzas", "CAF");
			SearchAndReplace(_curso, "Informática de oficina", "CIO");
			SearchAndReplace(_curso, "Xestión administrativa", "CXA");
			SearchAndReplace(_curso, "Ensinanza secundaria obrigatoria", "ESO");
			return _curso;
		}
		static const std::string none = "";
		return none;
	}

	int isNull() const {
		return (_nome.length() + _apelido1.length() + _apelido2.length() + _curso.length() + _grupo.length()) == 0;
	}

	int isInvalid() const {
		if(_nome.length() == 0) return 1;
		if(_apelido1.length() == 0 && _apelido2.length() == 0) return 1;
		if(_curso.length() == 0) return 1;
		if(_grupo.length() == 0) return 1;
		return 0;
	}

	std::string getPass() const {
		// return rand() % 10000;
		// Usamos una función hash determinista
		int cod3 = _nome.length();
		int cod2 = _apelido1.length();
		int cod1 = _apelido2.length();
		for(char c : _apelido2) cod1 = cod1 * 10 + c;
		for(char c : _apelido1) cod2 = cod2 * 10 + c;
		for(char c : _nome) cod3 = cod3 * 10 + c;
		int codXor = cod1 ^ cod2 ^cod3;
		int codNum = (codXor / 100000 + codXor) % 100000;
		std::string password = _nome + std::to_string(codNum);
		while(password.length() < 9) password += "0";
		return TextClean(password);
	}

	std::string getFullName() const {
		std::string fullName = _nome;
		if(_apelido1.length()) fullName += " " + _apelido1;
		if(_apelido2.length()) fullName += " " + _apelido2;
		return fullName;
	}

	std::string getCorreo() const {
		std::string correo = _nome;
		if(dotLess) SearchAndReplace(correo, " ", ".");
		if(_apelido1.length()) correo += "." + _apelido1;
		if(_apelido2.length()) {
			if(!dotLess) correo += ".";
			correo += _apelido2;
		}
		correo += "@iesafonsoxcambre.com";
		return TextClean(correo);
	}

	std::string to_csv() const {
		std::string apelido = _apelido1;
		std::string unidad = _curso + "_" + _grupo;// + "_" + getYear();
		if(_apelido2.length()) apelido += " " + _apelido2;
		return _nome + "," + apelido + "," + getCorreo() + "," + getPass() + ",/Alumnos/" + unidad + ",true\n";
	}

	std::string to_html() const {
		std::string head = "<TR> ", tail = "</TR>\n";
		return head + "<TD> " + _nome + "</TD> <TD> " + _apelido1
			+ "</TD> <TD> " + _apelido2 + "</TD> <TD> " + getCorreo()
			+ "</TD> <TD> <B> " + getPass() + "</B> </TD> " + tail;
	}

	std::string to_tex() const {
		std::string unidad = _curso + "_" + _grupo;
		std::string login = getCorreo();
		SearchAndReplace(login, "@iesafonsoxcambre.com", "");
		std::string head = "\\FICHA{", tail = "}\n";
		return head + unidad +"}{" + getFullName() + "}{" + login + "}{" + getPass() + tail;
	}

	std::string to_string() const {
		return _nome + " " + _apelido1 + " " + _apelido2 + " (" + _curso + " " + _grupo + ")\n";
	}

private:
	std::string _nome;
	std::string _apelido1;
	std::string _apelido2;
	std::string _curso;
	std::string _grupo;
};

class DataBase {

public:

	DataBase(const char* fName) : _fileCSV(0), _currentRow(0) {
		if(!fName || !fName[0])
			exit(printf("%s: Invalid file name\n", exeName));

		if(strstr(fName, ".xls")) {
			printf("%s: Parsing '%s' as Excel\n", exeName, fName);
			exit(printf("%s: Not implemented\n", exeName));
		}

		if(strstr(fName, ".csv")) {
			printf("%s: Parsing '%s' as CSV\n", exeName, fName);
			_fileCSV = fopen(fName, "r");
			if(!_fileCSV)
				exit(printf("%s: Error opening file '%s'\n", exeName, fName));
			return;
		}

		exit(printf("%s: Check extension of file '%s'\n", exeName, fName));
	}

	std::vector<std::string> nextRow() {
		std::vector<std::string> retVal;
		if(_fileCSV && !feof(_fileCSV)) {
			// Para ficheros CSV parseamos con STRTOK
			char tmp[1024];
			const char SEPARATOR = ';';
			if(fgets(tmp, 1000, _fileCSV)) {
				// Replace special line end terminators
				char* terminator = strpbrk(tmp, "\r\n");
				if(terminator) terminator[0] = terminator[1] = 0;

				// Perform STRTOK, but keeping consecutive tokens
				const char* token_end = tmp;
				for(const char* token_ini = tmp; *token_ini; token_ini = token_end + 1) {
					for(token_end = token_ini; *token_end; token_end++)
						if(*token_end == SEPARATOR) break;
					retVal.push_back(std::string(token_ini, token_end));
				}
			}
		} else {
			exit(printf("%s: XLS not implemented\n", exeName));
		}
		for(std::string &field : retVal)
			SearchAndReplace(field, "\"", "");
		return retVal;
	}

	int checkLabels() {
		// Contador para comprobar que la cabecera tiene los campos requeridos
		int cont = 0;
		for(std::string label : _labels) {
			if(label == "APELIDO1" || label == "Primeiro apelido") cont |= 0x01;
			if(label == "APELIDO2" || label == "Segundo apelido") cont |= 0x02;
			if(label == "NOME" || label == "Nome alumno") cont |= 0x04;
			if(label == "CURSO" || label == "Curso") cont |= 0x08;
			if(label == "GRUPO" || label == "Grupo") cont |= 0x10;
		}
		return cont == 0x1F;
	}

	int readLabels() {
		_labels = nextRow();
		if(!_fileCSV) {
			if(!checkLabels()) {
				printf("%s: Aviso, intentando omitir cabecera de importaci�n CSV\n", exeName);
				_labels = nextRow();
			}
		}

		if(!checkLabels())
			exit(printf("%s: Error, los campos no coinciden\n", exeName));
		return _labels.size();
	}

	std::string getLabel(unsigned pos) {
		if(pos < _labels.size())
			return _labels[pos];
		printf("%s: AVISO, lectura de etiqueta incorrecta\n", exeName);
		return "";
	}

	std::vector<Estudiante>& readStudents() {
		int numErrores = 0;
		printf("%s: Cargando tabla de estudiantes...\n", exeName);

		for(std::vector< std::string> row = nextRow(); row.size() > 0; row = nextRow()) {

			// Para cada estudiante establecemos sus atributos
			int cont = 0;
			Estudiante est;
			for(auto cell : row)
				est.set(getLabel(cont++), cell);

			// Comprobar registros invalidos
			if(est.isNull()) { if(_fileCSV) continue; else break; }
			if(est.isInvalid()) {
				numErrores++;
				if(numErrores == 1)
					printf("%s: AVISO, se han encontrado registros incompletos:\n", exeName);
				std::cout << est.to_string();
				if(numErrores > 20)
					exit(printf("%s: Error, revisar registros\n", exeName));
			} else {
				_estudiantes.push_back(est);
			}
			//std::cout << est.to_csv() << "\n";
		}
		return _estudiantes;
	}

private:

	FILE *_fileCSV;
	int _currentRow;
	std::vector<std::string> _labels;
	std::vector<Estudiante> _estudiantes;
};

void fileHeader(FILE* fOut) {
	if(!fOut) return;
	fprintf(fOut, "First Name [Required],Last Name [Required],"
		"Email Address [Required],Password [Required],"
		"Org Unit Path [Required], Change Password at Next Sign-In\n");
}

int group2HTML(std::vector<Estudiante> estudiantes, std::string nombreGrupo) {
	// Abrimos los ficheros de salida HTML
	std::string nombreHTML = nombreGrupo + ".html";
	FILE* fHTML = fopen(nombreHTML.c_str(), "w");
	if(!fHTML) exit(printf("%s: Error al crear fichero HTML '%s'\n", exeName, nombreHTML.data()));
	fprintf(fHTML, "<html> <head> <meta charset=\"utf-8\"/> </head> "
		"<body> <h2> Grupo: %s </h2> <table>\n",
		nombreGrupo.c_str());

	// Abrimos los ficheros de salida CSV
	std::string nombreCSV = nombreGrupo + ".csv";
	FILE* fCSV = fopen(nombreCSV.c_str(), "w");
	if(!fCSV) exit(printf("%s: Error al crear fichero CSV '%s'\n", exeName, nombreCSV.data()));
	fileHeader(fCSV);

	// Abrimos los ficheros de salida TEX
	std::string nombreTEX = nombreGrupo + ".tex";
	FILE* fTEX = fopen(nombreTEX.c_str(), "w");
	if(!fTEX) exit(printf("%s: Error al crear fichero TEX '%s'\n", exeName, nombreTEX.data()));
	fprintf(fTEX, "\\input{header.tex}\n\n\\begin{document}\n\\small\n\n");

	// Creamos los ficheros de salida
	int cont = 0;
	for(const Estudiante &est : estudiantes) {
		std::string esteGrupo = est.getCurso() + "_" + est.getGrupo();
		if(esteGrupo != nombreGrupo) continue;
		cont++;
		// Para subconjunto de alumnos de nombre compuesto
		// if(dotLess && est.getNombre().find(' ') == std::string::npos) continue;
		fputs(est.to_html().c_str(), fHTML);
		fputs("<tr> <td> &nbsp; </td> </tr>\n", fHTML);
		fputs("<tr> <td> &nbsp; </td> </tr>\n", fHTML);
		fputs(est.to_tex().c_str(), fTEX);
		fputs(est.to_csv().c_str(), fCSV);
		if(glbFile) fputs(est.to_csv().c_str(), glbFile);
	}

	// Cerramos los ficheros
	fputs("</table> </body> </html>\n", fHTML);
	fclose(fHTML);

	fputs("\n\\end{document}", fTEX);
	fclose(fTEX);
	std::string cmd = "pdflatex " + nombreTEX + ">> latex.log";
	system(cmd.data());

	// fputs("", fCSV);
	fclose(fCSV);

	printf("%s: %i registros escritos\n", exeName, cont);
	return 0;
}

int main(int argc, char* argv[]) {

	srand(time(NULL));
	for(char c = 'a'; c <= 'z'; c++) {
		conv[c] = c;
		conv[toupper(c)] = c;
	}
	for(char c = '0'; c <= '9'; c++)
		conv[c] = c;

	// Obtener el nombre del ejecutable para mostrar mensajes
	exeName = strrchr(argv[0], '/');
	exeName = exeName ? exeName+1 : argv[0];

	// Si no se han aportado parametros mostrar el modo de uso
	if(argc < 2) {
		printf("Usage: %s <file> [numGrupo]\n", exeName);
		printf("\t<file> puede ser .xls, .xlsx o .csv\n");
		printf("\tSi no se especifica [numGrupo] lista los grupos\n");
		printf("\t[numGroupo] puede ser /all para generar todos\n");
		printf("\tpuede añadirse -dot al final para omitir punto en apellido\n");
		return 0;
	}

	// Intentar abrir el fichero en función del tipo
	DataBase datos(argv[1]);

	// Cargamos las etiquetas de las columnas de la base de datos
	datos.readLabels();

	// Creamos el array de objetos de estudiantes
	std::vector<Estudiante> estudiantes = datos.readStudents();

	// Analizamos los estudiantes para obtener el conjunto de grupos
	std::map<std::string, int> grupos;
	for(const Estudiante &est : estudiantes)
		grupos[est.getCurso() + "_" + est.getGrupo()]++;

	// Si no se especifico el grupo mostramos un mensaje de error
	int cont = 1;
	if(argc == 2) {
		printf("Seleccione un grupo:\n");
		for(const auto &pair : grupos)
			printf("%2i: %8s (%2i)\n", cont++,
				pair.first.data(), pair.second);
		exit(0);
	}

	if(argc == 4 && argv[3][0] == '-' && argv[3][1] == 'd') {
		printf("Omitir punto en apellido activado\n");
		dotLess = 1;
	}

	// Si se desea procesar todos los grupos iteramos sobre el mapa
	if(argv[2][0] == '/' && argv[2][1] == 'a') {
		glbFile = fopen("all.csv", "w");
		if(!glbFile) puts("ERROR: Could not open 'all.csv'");
		fileHeader(glbFile);

		printf("Todos los grupos seleccionados\n");
		for(auto grupo : grupos)
			group2HTML(estudiantes, grupo.first);
		if(glbFile) fclose(glbFile);
		return 0;
	}

	// Si se especifico un grupo comprobamos que se encuentre dentro de rango
	cont = atoi(argv[2]);
	if(cont < 1 || cont > (int)grupos.size()) {
		return printf("%s: Error, %i grupo no valido\n", exeName, cont);
	}

	// Obtenemos la etiqueta correspondiente al identificador numérico
	auto it = grupos.begin();
	while(--cont) it++;
	std::string nombreGrupo = it->first;
	printf("Seleccionado: %s\n", nombreGrupo.c_str());
	group2HTML(estudiantes, nombreGrupo);
	return 0;
}
