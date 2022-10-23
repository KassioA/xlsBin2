# xlsBin2

For English documentation, **[click here](#en-us)**.

Para a documentação em português, **[clique aqui](#pt-br)**.

# (EN-US)

A tool for converting .bin files to .csv, and vice versa, using Lua scripts.

## 🚀 Getting Started

These instructions will allow you to get a working copy of the project on your local machine for development and testing purposes.

See **[Operation](#-operation)** to understand how the project works

### 📋 Prerequisites

What things do you need to install the software?

```
Visual Studio with C++17 or higher
```

### 🔧 Operation

xlsBin2 was developed to work through Lua scripts, currently in version 5.4.

Implemented variables:

```
• filename => Variable containing the name of the file, without extension.
• fileextension => Variable containing the file extension.
• filesize => Variable containing the size of the binary file.
• headersize => Variable containing the size of the header, defined in the .st file.
• footersize => Variable containing the footer size, defined in the .st file.
```

Functions:

```
• loadFileBuf => Load the binary file into the shared buffer.
• saveFileBuf => Saves the shared buffer to a binary file.
• toCSV => Performs the conversion of the binary content of the shared buffer to CSV.
• toBIN => Performs the conversion of the CSV file to binary and stores in the shared buffer.

• getInput => Prompts the user to type something, and returns the typed content.
```


Lua script example:

```
nativeextension = ".bin" -- Native file extension
finalextension = ".csv" -- Output extension

if (fileextension == nativeextension) then
    print("Loading binary file")
    loadFileBuf() -- Load binary file into shared buffer

    print("Converting to CSV...")
    toCSV() -- Convert the binary file to CSV

    print("Successfully converted.")


elseif (fileextension == finalextension) then

    print("Converting to BIN...")
    toBIN() -- Convert the CSV file to binary and stores in shared buffer

    print("Saving binary file...")
    saveFileBuf("enc_" .. filename .. nativeextension) -- Save the contents of the shared buffer to a binary file
    
    print("Successfully converted.")

end
```

xlsBin2 has its own file extension (.st), these files will contain binary file information:

```
• Name of the file containing the script to be used
• Header Size
• Footer size
• Item structure
```

For the conversion, a .st file with the same name as the input file is required.

An Example.bin file will only be converted by xlsBin2 if there is Example.st in the data folder.

Example .st file:

```
##scriptfile=example.lua
##headersize=10
##footersize=20

##beginstruct
char name[50], lastname[50]
uint age, birthyear
##endstruct
```

Types supported by xlsBin2:

```
char => Used for text, occupies 1 byte.
byte => Occupies 1 byte. (-128 to 127)
uchar => Occupies 1 byte. (0 to 255)
short => Occupies 2 bytes. (-32768 to 32767)
ushort => Occupies 2 bytes. (0 to 65535)
int => Occupies 4 bytes. (-2147483648 to 2147483647)
uint => Occupies 4 bytes. (0 to 4294967295)
long => Occupies 8 bytes. (-9223372036854775808 to 9223372036854775807)
ulong => Occupies 8 bytes. (0 to 18446744073709551615)
```

## ⚙️ Running the tests

For the sake of explanation, let's do a little more complex demo.

We have an ItemList.bin file, which contains the list of items for a game.

It has a simple encoding, which basically adds to each byte of the file, the modulus of its position with 5 (pos % 5).

In addition, it has a header and footer, of sizes 32 and 128, respectively, containing information not relevant to this demonstration. Both are not coded by nature.

ItemList.st file:

```
##scriptfile=ItemList.lua
##headersize=32
##footersize=128

##beginstruct
char Name[64]
char Description[128]
uint ItemType
int Attack, Defense
int Price
##endstruct
```

ItemList.lua file:

```
nativeextension = ".bin" -- Native file extension
finalextension = ".csv" -- Output extension

function crypt(sign) -- Encoding/decoding function of the bin file in question. Parameter sign: 1 to decode, -1 to encode.
    
    nfile = sharedbuf() -- Initialization of shared buffer between xlsBin2 and Lua.
    
    for i=1 + headersize, filesize - footersize, 1 -- Loop ignoring header and footer
    do
        nfile[i] = nfile[i] - sign * ((i - 1) % 5) -- Encoding/decoding operation
    end
    
end

if (fileextension == nativeextension) then
    loadFileBuf() -- Load binary file into shared buffer

    print("Decoding file...")
    crypt(1) -- Decode the binary file

    print("Saving binary file decoded...")
    saveFileBuf("1dec_" .. filename .. nativeextension) -- Save binary file decoded

    print("Converting to CSV...")
    toCSV() -- Convert binary file to CSV

    print("Successfully converted.")


elseif (fileextension == finalextension) then

    print("Converting to binary...")
    toBIN() -- Convert CSV file to binary and load into shared buffer

    print("Saving decoded binary file...")
    saveFileBuf("2dec_" .. filename .. nativeextension) -- Save the decoded binary file

    print("Encoding file...")
    crypt(-1); -- Encode binary file

    print("How do you want to save the result file?")
    fname = getInput()
    print("Saving encoded file.")
    saveFileBuf(fname) -- Save the encoded binary file
    
    print("Successfully converted.")

end
```

## 🛠️ Built with

Mention the tools you used to create your project

* [Visual Studio 2022](https://visualstudio.microsoft.com/) - IDE used in programming (C++)
* [Lua](https://www.lua.org/) - Scripting language used.

## ✒️ Authors

Mention all those who helped lift the project from its inception

* **Kassio Amorim** - *Programming and documentation* - [KassioA](https://github.com/KassioA)
* **HuninHune** - *Initial Idea*

## 📄 License

This project is under the MIT license - see the file [LICENSE.md](https://github.com/KassioA/xlsBin2/blob/master/LICENSE.md) for details.

## 🎁 Expressions of Gratitude

* Tell others about this project 📢;
* Invite someone from the team for a beer 🍺;
* A public thanks;
* etc.

# (PT-BR)

Uma ferramenta para conversão de arquivos .bin para .csv, e vice versa, utilizando scripts Lua.

## 🚀 Começando

Essas instruções permitirão que você obtenha uma cópia do projeto em operação na sua máquina local para fins de desenvolvimento e teste.

Consulte **[Funcionamento](#-funcionamento)** para entender o funcionamento do projeto

### 📋 Pré-requisitos

De que coisas você precisa para instalar o software?

```
Visual Studio com C++17 ou superior
```

### 🔧 Funcionamento

xlsBin2 foi desenvolvido para trabalhar através de scripts Lua, atualmente na versão 5.4.

Variáveis implantadas:

```
• filename => Variável contendo o nome do arquivo, sem extensão.
• fileextension => Variável contendo a extensão do arquivo.
• filesize => Variável contendo o tamanho do arquivo binário.
• headersize => Variável contendo o tamanho do cabeçalho, definido no arquivo .st.
• footersize => Variável contendo o tamanho do rodapé, definido no arquivo .st.
```

Funções:

```
• loadFileBuf => Carrega o arquivo binário no buffer compartilhado.
• saveFileBuf => Salva o buffer compartilhado em um arquivo binário.
• toCSV => Executa a conversão do conteúdo binário do buffer compartilhado para CSV.
• toBIN => Executa a conversão do arquivo CSV para binário e armazena no buffer compartilhado.

• getInput => Solicita que o usuário digite algo, e retorna o conteúdo digitado.
```

Exemplo de script Lua:

```
nativeextension = ".bin" -- Extensão do arquivo nativo
finalextension = ".csv" -- Extensão de saída

if (fileextension == nativeextension) then
    print("Loading binary file")
    loadFileBuf() -- Carrega o arquivo binário no buffer compartilhado

    print("Converting to CSV...")
    toCSV() -- Converte o arquivo binário para CSV

    print("Successfully converted.")


elseif (fileextension == finalextension) then

    print("Converting to BIN...")
    toBIN() -- Converte o arquivo CSV para binário e armazena no buffer compartilhado

    print("Saving binary file...")
    saveFileBuf("enc_" .. filename .. nativeextension) -- Salva o conteúdo do buffer compartilhado em arquivo binário
    
    print("Successfully converted.")

end
```

xlsBin2 tem uma extensão de arquivos própria(.st), esses arquivos irão conter informações do arquivo binário:

```
• Nome do arquivo contendo script a ser utilizado
• Tamanho do cabeçalho
• Tamanho do rodapé
• Estrutura de item
```

Para a conversão, é necessário um arquivo .st de mesmo nome do arquivo de entrada.

Um arquivo Exemplo.bin só será convertido pelo xlsBin2 se existir Exemplo.st na pasta data.

Exemplo de arquivo .st:

```
##scriptfile=exemplo.lua
##headersize=10
##footersize=20

##beginstruct
char nome[50], sobrenome[50]
uint idade, anonascimento
##endstruct
```

Tipos suportados pelo xlsBin2:

```
char => Utilizado para texto, ocupa 1 byte. 
byte => Ocupa 1 byte. (-128 a 127)
uchar => Ocupa 1 byte. (0 a 255)
short => Ocupa 2 bytes. (-32768 a 32767)
ushort => Ocupa 2 bytes. (0 a 65535)
int => Ocupa 4 bytes. (-2147483648 a 2147483647)
uint => Ocupa 4 bytes. (0 a 4294967295)
long => Ocupa 8 bytes. (-9223372036854775808 a 9223372036854775807)
ulong => Ocupa 8 bytes. (0 a 18446744073709551615)
```

## ⚙️ Executando os testes

Para fins de explicação, vamos fazer uma demonstração um pouco mais complexa.

Temos um arquivo ItemList.bin, que contém a lista de itens de um jogo.

Ele possui uma codificação simples, que basicamente soma a cada byte do arquivo, o módulo da sua posição com 5 (pos % 5).

Além disso, ele possui um cabeçalho e rodapé, de tamanhos 32 e 128, respectivamente, contendo informações não relevantes para esta demonstração. Ambos não são codificados, por natureza.

Arquivo ItemList.st:

```
##scriptfile=ItemList.lua
##headersize=32
##footersize=128

##beginstruct
char Nome[64]
char Descricao[128]
uint ItemType
int Ataque, Defesa
int Preco
##endstruct
```

Arquivo ItemList.lua:

```
nativeextension = ".bin" -- Extensão do arquivo nativo
finalextension = ".csv" -- Extensão de saída

function crypt(sign) -- Função de codificação/decodificação do arquivo bin em questão. Parâmetro sign: 1 para decodificar, -1 para codificar.
    
    nfile = sharedbuf() -- Inicialização do buffer compartilhado entre o xlsBin2 e o Lua.
    
    for i=1 + headersize, filesize - footersize, 1 -- Loop ignorando o cabeçalho e rodapé
    do
        nfile[i] = nfile[i] - sign * ((i - 1) % 5) -- Operação de codificação/decodificação
    end
    
end

if (fileextension == nativeextension) then
    loadFileBuf() -- Carrega o arquivo binário no buffer compartilhado

    print("Decodificando arquivo...")
    crypt(1) -- Decodifica o arquivo binário

    print("Salvando arquivo binário decodificado...")
    saveFileBuf("1dec_" .. filename .. nativeextension) -- Salva o arquivo binário decodificado

    print("Convertendo para CSV...")
    toCSV() -- Converte o arquivo binário para CSV

    print("Convertido com sucesso.")


elseif (fileextension == finalextension) then

    print("Convertendo para binário...")
    toBIN() -- Converte o arquivo CSV para binário e carrega no buffer compartilhado

    print("Salvando arquivo binário decodificado...")
    saveFileBuf("2dec_" .. filename .. nativeextension) -- Salva o arquivo binário decodificado

    print("Codificando arquivo...")
    crypt(-1); -- Codifica o arquivo binário

    print("Como deseja salvar o arquivo resultado?")
    fname = getInput()
    print("Salvando arquivo codificado.")
    saveFileBuf(fname) -- Salva o arquivo binário codificado
    
    print("Convertido com sucesso.")

end
```

## 🛠️ Construído com

Mencione as ferramentas que você usou para criar seu projeto

* [Visual Studio 2022](https://visualstudio.microsoft.com/pt-br/) - IDE utilizada na programação(C++)
* [Lua](https://www.lua.org/portugues.html) - Linguagem de Script utilizada.

## ✒️ Autores

Mencione todos aqueles que ajudaram a levantar o projeto desde o seu início

* **Kassio Amorim** - *Programação e documentação* - [KassioA](https://github.com/KassioA)
* **HuninHune** - *Ideia inicial*


## 📄 Licença

Este projeto está sob a licença MIT - veja o arquivo [LICENSE.md](https://github.com/KassioA/xlsBin2/blob/master/LICENSE.md) para detalhes.

## 🎁 Expressões de gratidão

* Conte a outras pessoas sobre este projeto 📢;
* Convide alguém da equipe para uma cerveja 🍺;
* Um agradecimento publicamente 🫂;
* etc.
