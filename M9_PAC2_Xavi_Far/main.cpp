#include <iostream>
#include <string>
#include <sqlite3.h>
using namespace std;

// Función para ejecutar una consulta SQL
bool ejecutarConsulta(sqlite3* db, const string& consulta) {
    char* mensajeError = nullptr;
    int resultado = sqlite3_exec(db, consulta.c_str(), nullptr, nullptr, &mensajeError);

    if (resultado != SQLITE_OK) {
        cerr << "Error en la consulta SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
        return false;
    }
    return true;
}

// Función para configurar la base de datos y crear la tabla de usuarios si no existe
void configurarBaseDatos(sqlite3* db) {
    string consulta = "CREATE TABLE IF NOT EXISTS Usuarios ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Nombre TEXT NOT NULL, "
                      "Email TEXT NOT NULL UNIQUE);"; // Cambiamos Edat por Email
    ejecutarConsulta(db, consulta);
}

// Función para agregar un nuevo usuario a la base de datos
void agregarUsuario(sqlite3* db) {
    string nombre, email;

    cout << "Introduce el nombre del nuevo usuario: ";
    cin.ignore();  // Limpiar el buffer
    getline(cin, nombre);
    cout << "Introduce el email del usuario: ";
    getline(cin, email);

    string consulta = "INSERT INTO Usuarios (Nombre, Email) VALUES ('" + nombre + "', '" + email + "');";
    if (ejecutarConsulta(db, consulta)) {
        cout << "Usuario agregado exitosamente.\n";
    }
}

// Función para eliminar un usuario de la base de datos
void eliminarUsuarioPorID(sqlite3* db) {
    int id;
    cout << "Introduce el ID del usuario a eliminar: ";
    cin >> id;

    string consulta = "DELETE FROM Usuarios WHERE ID = " + to_string(id) + ";";
    if (ejecutarConsulta(db, consulta)) {
        cout << "Usuario eliminado exitosamente.\n";
    }
}

// Función para actualizar la información de un usuario
void actualizarUsuario(sqlite3* db) {
    int id;
    string nuevoNombre, nuevoEmail;

    cout << "Introduce el ID del usuario a modificar: ";
    cin >> id;
    cout << "Introduce el nuevo nombre: ";
    cin.ignore();  // Limpiar el buffer
    getline(cin, nuevoNombre);
    cout << "Introduce el nuevo email: ";
    getline(cin, nuevoEmail);

    string consulta = "UPDATE Usuarios SET Nombre = '" + nuevoNombre + "', Email = '" + nuevoEmail + "' WHERE ID = " + to_string(id) + ";";
    if (ejecutarConsulta(db, consulta)) {
        cout << "Usuario actualizado exitosamente.\n";
    }
}

// Callback para mostrar los usuarios
static int mostrarUsuariosCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " ";
    }
    cout << endl;
    return 0;
}

// Función para mostrar todos los usuarios de la base de datos
void mostrarUsuarios(sqlite3* db) {
    string consulta = "SELECT * FROM Usuarios;";
    cout << "Usuarios en la base de datos:\n";
    sqlite3_exec(db, consulta.c_str(), mostrarUsuariosCallback, nullptr, nullptr);
}

int main() {
    // Conectar a la base de datos SQLite
    sqlite3* db;
    int exit = sqlite3_open("usuarios.db", &db);

    if (exit) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return -1;
    } else {
        cout << "Conexión a la base de datos exitosa.\n";
    }

    // Configurar la base de datos (crear tabla si no existe)
    configurarBaseDatos(db);

    int opcion;

    do {
        cout << "\n--- Menú Base de Datos de Usuarios ---\n";
        cout << "1. Mostrar todos los usuarios\n";
        cout << "2. Añadir un usuario\n";
        cout << "3. Eliminar un usuario\n";
        cout << "4. Modificar un usuario\n";
        cout << "5. Salir\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarUsuarios(db);
                break;
            case 2:
                agregarUsuario(db);
                break;
            case 3:
                eliminarUsuarioPorID(db);
                break;
            case 4:
                actualizarUsuario(db);
                break;
            case 5:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción no válida. Inténtalo de nuevo.\n";
        }
    } while (opcion != 5);

    // Cerrar la conexión a la base de datos
    sqlite3_close(db);

    return 0;
}
