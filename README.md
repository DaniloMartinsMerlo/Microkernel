# Microkernel

## Vídeo de demonstração

<iframe width="560" height="315" src="https://www.youtube.com/embed/l-2tJM37S9Y?si=cWEHJz70tTsiCFj4" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

📎 [Assistir no YouTube](https://www.youtube.com/watch?v=l-2tJM37S9Y)

---

## Como executar

### 1. Compilar  
Antes de executar, é necessário compilar todas as dependências. Estando na raiz do repositório, execute os seguintes comandos:

```zsh
nasm -f elf32 kernel.asm -o kasm.o
gcc -m32 -c -fno-stack-protector kernel.c -o kc.o
gcc -m32 -c -fno-stack-protector termo_game.c -o termo.o
gcc -m32 -c -fno-stack-protector string_utils.c -o string_utils.o
gcc -m32 -c -fno-stack-protector keyboard_map.c -o keyboard.o
```

### 2. Linkar

&emsp; Após a compilação, é preciso linkar os arquivos gerados para formar o kernel. Para isso, execute:

```zsh
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o termo.o string_utils.o keyboard.o
```

### 3. Executar

&emsp; Agora é possível rodar o kernel. Recomenda-se utilizar o QEMU:

```zsh
qemu-system-i386 -kernel kernel
```

## Como jogar

&emsp;O jogo funciona de forma semelhante ao Termo/Wordle.

- Letra verde → a letra está correta e na posição correta.

- Letra laranja → a letra existe na palavra, mas está na posição errada.

- Letra cinza → a letra não está presente na palavra.

&emsp; O objetivo é descobrir a palavra correta em até 6 tentativas.