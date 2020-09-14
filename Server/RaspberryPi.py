import json
import math
from raspi_lora import LoRa, ModemConfig
import pyrebase
 
    

def read_settings(): # Definição de como irá ler o arquivo JSON
    # Abre o .json configurado em modo de leitura 
    with open("Settings.json", 'r') as settings:
        # Carrega os dados do arquivo como um diretório e retorna isso
        settings_data = json.loads(settings.read())
        return settings_data

def parse_payload(payload): # Definição que decodifica a mensagem e a recorta

    # Decodifica a mensagem de bytes
    raw_data = bytes(payload.message).decode('utf-8')

    # Separa as mensagens nos caracteres "$"
    parsed_data = [str(data) for data in raw_data.split('$')]

    # retorna a data separada
    return parsed_data



def calculate_volume(shape, data, data2): # Definição que identifica qual forma geométrica é e define qual formula usar

    volume_formulas = {'Cilindrica': cilindrica_volume, 'Tronco de Piramide': tronco_piramide_volume, 'Paralelogramo': paralelogramo_volume}
    return volume_formulas[shape](data, data2)

def cilindrica_volume(data, data2): # Definição para as lixeiras de forma cilindrica

    altura_teto = data['Altura ate o Teto'] # Lê qual é a altura até o teto do arquivo auxiliar .json
    raio = data['Raio'] # Lê qual é o raio do arquivo auxiliar .json
    media = data['n# Sensores'] # Lê qual é numero de sensores ultrassonicos para realizar a média da altura
    converse = float(data2) # Lê da mensagem recebida qual é o dado dos sensores ultrassonicos e os converte para o tipo Float
    altura = converse/media # Realiza a média

    # Retorna o volume em cilindro
    return (2*math.pi*raio**2)*(altura_teto - altura)

def tronco_piramide_volume(data, data2): # Definição para as lixeiras na forma de um tronco de piramide

    altura_teto = data['Altura ate o Teto'] #Lê qual é a altura do arquivo auxiliar .json
    area_menor = data['Comprimento da Base Menor'] * data['Largura da Base Menor'] #Lê qual é area menor do arquivo auxiliar .json
    area_maior = data['Comprimento da Base Maior'] * data['Largura da Base Maior'] #Lê qual é area maior do arquivo auxiliar .json
    media = data['n# Sensores']  # Lê qual é numero de sensores ultrassonicos para realizar a média da altura
    converse = float(data2) # Lê da mensagem recebida qual é o dado dos sensores ultrassonicos e os converte para o tipo Float
    altura = converse/media # Realiza a média
    
    # Retorona o volume de um tronco de piramide
    return ((altura_teto - altura)/3) * (area_maior + (math.sqrt(area_maior*area_menor) + area_menor))

def paralelogramo_volume(data, data2): # Definição para as lixeiras na forma de um paralelogramo

    altura_teto = data['Altura ate o Teto'] # Lê qual é a altura até o teto do arquivo auxiliar .json
    comprimento = data['Comprimento da Base'] # Lê qual é o comprimento do arquivo auxiliar .json
    largura = data['Largura da Base'] # Lê qual é a largura do arquivo auxiliar 
    media = data['n# Sensores']  # Lê qual é numero de sensores ultrassonicos para realizar a média da altura
    converse = float(data2) # Lê da mensagem recebida qual é o dado dos sensores ultrassonicos e os converte para o tipo Float
    altura = converse/media # Realiza a média

    #Retorna o volume do paralelogramo
    return comprimento*largura*(altura_teto - altura)

def weight(data, data2): # Definição do peso
    tare = data['Peso da Lixeira Vazia'] # Lê qual é o peso da lixeira vazia do arquivo auxiliar .json
    peso = float(data2) - tare # Realizar uma tara desse peso
    
    # Retorna o peso já feito a tara
    return peso

def on_recv(payload): # Definição de como é o recebimento das mensagens e todo o tratamento até o envio ao Firebase

    
    sender_id = payload.header_from # Armazena o ID da lixeira que mandou a mensagem

    parsed_data = parse_payload(payload) # Chama a def que decodifica e separa as mensagens
 

    print("De: ", payload.header_from) # Printa o ID
    print("Mensagem: ", payload.message) # Printa a mensagem
    
    print("Peso: ", parsed_data[1]) # Printa o peso
    print("Altura: ", parsed_data[2]) # Printa a altura 

    
    settings = read_settings() # Lê o arquivo .json para pegar a lista de todas as lixeiras, seus nomes e o nome do condominio
    trash_cans = settings['information']['Lixeiras'] # Armazena a lista das lixeiras
    condominio = settings['information']['Nome do Condominio'] # Armazena o nome do condominio

    # Já que temos a lista das lixeiras e seus IDs
    # Podemos pegar os nomes e dados para enviar via LoRA
    for can in trash_cans: 
        if can["ID"] == sender_id: # Se o ID coincidir
            sender_name = can["Nome"] # Armazeno o nome da Lixeira
            print("Id é : ", target_can_id, type(target_can_id))
            settings_data = read_settings() #Lê novamente o arquivo .json

            lixeiras = settings_data['information']['Lixeiras'] 
            lixeiras = [ {'ID': lixeira['ID'], 'Forma Geometrica': lixeira['Dados']['Forma Geometrica'], 'Dados': lixeira['Dados']} for lixeira in lixeiras if lixeira['ID'] == sender_id]
            print(lixeiras)
            
            volume = [calculate_volume(lixeira['Forma Geometrica'], lixeira['Dados'], parsed_data[2]) for lixeira in lixeiras] # chama a def que irá calcular o volume
            print("o volume é: ", volume)
            peso = [weight(lixeira['Dados'], parsed_data[1]) for lixeira in lixeiras] # chama a def que irá calcular o peso com tara
            print("O peso é: ", peso)

            db.child(condominio).child(sender_name).child('Peso').set(peso) #Envia o peso para o FireBase
            db.child(condominio).child(sender_name).child('Volume').set(volume) # Envia o volume para o FireBase
    

        

            
config = { #Configurações de acesso para o banco de dados
  "apiKey": "AIzaSyCWH1RjnADOMq_vjOJmelWc0uYOPZ8AX_0",
  "authDomain": "cyclefy-5f768.firebaseapp.com",
  "databaseURL": "https://cyclefy-5f768.firebaseio.com",
  "storageBucket": "cyclefy-5f768.appspot.com"
}

firebase = pyrebase.initialize_app(config) # inicia a comunicação usando as configurações
db = firebase.database() # Define o Banco de dados

lora = LoRa(0, 17, 1, freq=868, modem_config=ModemConfig.Bw31_25Cr48Sf512, tx_power=14, acks=True) # Configuração do LoRA

print("iniciando...")
lora.on_recv = on_recv # Chama a def que recebe, decodifica, separa e envia a mensagem para o Firebase
lora.set_mode_rx() # define o LoRa para modo de receptor continuo 
print("Finalizado")    





