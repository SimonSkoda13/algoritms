#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 42 // Maximálny počet kontaktov
#define MAX_LINE 100    // Maximálna dĺžka mena alebo telefónneho čísla

// Mapa číslic na zodpovedajúce znaky (T9)
char *keypad[] = {
    "+",    // 0
    "",     // 1 (nepoužíva sa)
    "abc",  // 2
    "def",  // 3
    "ghi",  // 4
    "jkl",  // 5    
    "mno",  // 6
    "pqrs", // 7
    "tuv",  // 8
    "wxyz"  // 9
};

// Štruktúra pre uchovanie kontaktu (meno a telefónne číslo)
typedef struct
{
    char name[MAX_LINE];  // Meno kontaktu
    char phone[MAX_LINE]; // Telefónne číslo kontaktu
} Contact;

// Pole pre uchovanie kontaktov
Contact contacts[MAX_CONTACTS];
int contact_count = 0; // Počítadlo počtu kontaktov

// Deklarácie funkcií
int matches_sequence(const char *str, const char *sequence);
int matches_sequence_with_skips(const char *str, const char *sequence);
int char_to_digit(char c);
int is_valid_sequence(const char *sequence);
void load_contacts();
void find_contacts(const char *sequence, int allow_skips);

// Funkcia pre konverziu znaku na zodpovedajúcu číslicu na klávesnici (T9)
int char_to_digit(char c)
{
    c = tolower(c); // Ignorovanie veľkosti písmen

    // Spracovanie symbolu '+' a jeho mapovanie na 0
    if (c == '+')
    {
        return 0; // Mapovanie '+' na '0'
    }

    // Spracovanie číslic '0'-'9'
    if (isdigit(c))
    {
        return c - '0'; // Konverzia znaku číslice na číslicu
    }

    // Prevod písmen na zodpovedajúce číslice podľa T9 klávesnice
    for (int i = 0; i < 10; i++)
    {
        if (strchr(keypad[i], c) != NULL)
        {
            return i;
        }
    }

    return -1; // Vrátenie -1, ak znak nezodpovedá žiadnej číslici
}

// Funkcia pre kontrolu, či je reťazec platná číselná sekvencia
int is_valid_sequence(const char *sequence)
{
    for (size_t i = 0; i < strlen(sequence); i++)
    {
        if (!isdigit(sequence[i])) // Ak nájdeme nečíselný znak, vrátime chybu
        {
            return 0;
        }
    }
    return 1;
}

// Funkcia pre hľadanie presných sekvencií (bez medzier)
int matches_sequence(const char *str, const char *sequence)
{
    size_t seq_len = strlen(sequence); // Dĺžka vstupnej sekvencie (napr. "0420")
    size_t str_len = strlen(str);      // Dĺžka reťazca, v ktorom hľadáme (napr. "+420123456678")

    // Skorý návrat, ak je reťazec kratší ako sekvencia
    if (str_len < seq_len)
    {
        return 0; // Nie je možné nájsť zhodu
    }

    // Iterácia cez reťazec na nájdenie miesta, kde by sa mohla sekvencia zhodovať
    for (size_t i = 0; i <= str_len - seq_len; i++)
    {
        int match = 1; // Predpokladáme, že zhoda je pravdivá

        // Porovnanie každého znaku sekvencie so zodpovedajúcou časťou reťazca
        for (size_t j = 0; j < seq_len; j++)
        {
            int digit = sequence[j] - '0';             // Konverzia znaku sekvencie na číslicu
            int str_digit = char_to_digit(str[i + j]); // Získanie zodpovedajúcej číslice pre telefónne číslo

            if (str_digit != digit)
            { // Ak sa nezhoduje, prerušíme cyklus
                match = 0;
                break;
            }
        }

        if (match)
        { // Ak nájdeme zhodu, vrátime 1
            return 1;
        }
    }

    return 0; // Ak sa nenašla žiadna zhoda, vrátime 0
}

// Funkcia pre hľadanie sekvencií s medzerami
int matches_sequence_with_skips(const char *str, const char *sequence)
{
    size_t seq_len = strlen(sequence);
    size_t str_len = strlen(str);

    size_t seq_index = 0; // Index v sekvencii

    // Prechádzanie celým reťazcom
    for (size_t i = 0; i < str_len; i++)
    {
        // Získanie číslice z aktuálneho znaku sekvencie
        int digit = sequence[seq_index] - '0';

        // Prevod znaku na číslicu podľa T9
        int str_digit = char_to_digit(str[i]);

        // Ak znak zodpovedá číslici, posunieme sa v sekvencii
        if (str_digit == digit)
        {
            seq_index++;
        }

        // Ak sme prešli celú sekvenciu, vrátime TRUE (1)
        if (seq_index == seq_len)
        {
            return 1;
        }
    }

    // Ak sme neprešli celú sekvenciu, vrátime FALSE (0)
    return 0;
}

// Načítanie kontaktov zo vstupu
void load_contacts()
{
    char name[MAX_LINE], phone[MAX_LINE];
    while (fgets(name, sizeof(name), stdin) && fgets(phone, sizeof(phone), stdin))
    {
        name[strcspn(name, "\n")] = '\0';   // Odstránenie znaku nového riadku z mena
        phone[strcspn(phone, "\n")] = '\0'; // Odstránenie znaku nového riadku z telefónneho čísla
        strcpy(contacts[contact_count].name, name);
        strcpy(contacts[contact_count].phone, phone);
        contact_count++;
    }
}

// Vyhľadanie kontaktov podľa sekvencie
void find_contacts(const char *sequence, int allow_skips)
{
    int found = 0;
    for (int i = 0; i < contact_count; i++)
    {
        int match = 0;

        // Hľadanie sekvencie v telefónnych číslach a menách
        if (allow_skips)
        {
            // Hľadanie s povolenými medzerami (ak je implementované)
            match = matches_sequence_with_skips(contacts[i].phone, sequence) ||
                    matches_sequence_with_skips(contacts[i].name, sequence);
        }
        else
        {
            // Presná zhoda
            match = matches_sequence(contacts[i].phone, sequence) ||
                    matches_sequence(contacts[i].name, sequence);
        }

        if (match)
        {
            printf("%s, %s\n", contacts[i].name, contacts[i].phone);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Not found\n");
    }
}

// Hlavná funkcia
int main(int argc, char *argv[])
{
    char *sequence = NULL;
    int allow_skips = 0; // Príznak pre povolenie medzier

    // Spracovanie argumentov
    if (argc > 1)
    {
        if (strcmp(argv[1], "-s") == 0 && argc > 2)
        {
            allow_skips = 1;
            sequence = argv[2];

            if (!is_valid_sequence(sequence)) // Kontrola, či sekvencia obsahuje iba číslice
            {
                fprintf(stderr, "Invalid argument: sequence must contain only digits.\n");
                return 1; // Vrátime chybu, ak sekvencia obsahuje neplatné znaky
            }
        }
        else if (argc > 2 && strcmp(argv[1], "-s"))
        {
            fprintf(stderr, "Invalid argument\n");
            return 1;
        }

        else if (isdigit(argv[1][0])) // Overenie, že prvý argument je číslo
        {
            sequence = argv[1];
            if (!is_valid_sequence(sequence)) // Kontrola platnosti sekvencie
            {
                fprintf(stderr, "Invalid argument: sequence must contain only digits.\n");
                return 1; // Vrátime chybu, ak sekvencia obsahuje neplatné znaky
            }
        }
        else
        {
            fprintf(stderr, "Invalid argument\n");
            return 1; // Návratový kód pre chybu
        }
    }

    load_contacts(); // Načítanie kontaktov

    if (sequence)
    {
        find_contacts(sequence, allow_skips); // Vyhľadanie kontaktov podľa sekvencie s/bez medzier
    }
    else
    {
        for (int i = 0; i < contact_count; i++)
        {
            printf("%s, %s\n", contacts[i].name, contacts[i].phone);
        }
    }

    return 0; // Úspešný návratový kód
}