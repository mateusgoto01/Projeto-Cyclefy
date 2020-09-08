import json 
import math
from raspi_lora import LoRa, ModemConfig
import pyrebase

def read_settings():

    # Open the json file with the settings data in read mode 
    with open("Settings.json", 'r') as settings:

        # Load the data from the file as a dictionary and return it 
        settings_data = json.loads(settings.read())
        return settings_data

def parse_payload(payload):

    # decode the data from bytes 
    raw_data = bytes(payload.message).decode('utf-8')

    # parse the data using $ as the delimiter 
    parsed_data = [str(data) for data in raw_data.split('$')]

    # return the parsed data 
    return parsed_data



def calculate_volume(shape, data):

    volume_formulas = {'Cilindrica': cilindrica_volume, 'Tronco de Piramide': tronco_piramide_volume, 'Paralelogramo': paralelogramo_volume}
    return volume_formulas[shape](data)

def cilindrica_volume(data):

    altura_teto = data['Altura ate o Teto']
    raio = data['Raio']
    media = parsed_data[2]/data['n# Sensores']

    # calculate the fraking volume here
    return (2*math.pi*raio**2)*(altura_teto - media)

def tronco_piramide_volume(data):

    altura_teto = data['Altura ate o Teto']
    area_menor = data['Area da Base Menor']
    area_maior = data['Area da Base Maior']
    media = parsed_data[2]/data['n# Sensores']
    
    return ((altura_teto - media)/3) * (area_maior + (math.sqrt(area_maior*area_menor) + area_menor))

def paralelogramo_volume(data):

    altura_teto = data['Altura ate o Teto']
    area = data['Area da Base']
    media = parsed_data[2]/data['n# Sensores']

    return area*(altura_teto - media)



def on_recv(payload):

    # Get the id form the sender form the payload 
    sender_id = payload.header_from

    print("From: ", payload.header_from)
    print("Received: ", payload.message)
    dados = bytes(payload.message).decode("utf-8")
    print(dados)
    parsed_data = [str(data) for data in dados.split('$')]
    print("Peso: ", parsed_list[1])
    print("Altura: ", parsed_list[2])

    # Read the json file to get a list of all trash cans and the name of the condomínio
    settings = read_settings()
    trash_cans = settings['information']['Lixeiras'] # ! Não é a melhor forma
    condominio = settings['information']['Nome do Condomínio']

    # Since we have a list with all the cans in the system and their respective IDs and Names
    # we can get the name of the one that send the message via LoRa 
    for can in trash_cans:
        if can["ID"] == sender_id:
            sender_name = can["Nome"]
            target_can_id = sender_name
            settings_data = read_settings()
            lixeiras = settings_data['information']['Lixeiras']
            lixeiras = [ {'ID': lixeira['ID'], 'Forma Geometrica': lixeira['Dados']['Forma Geometrica'], 'Dados': lixeira['Dados']} for lixeira in lixeiras if lixeira['ID'] == target_can_id]
            tare = data["Peso da Lixeira Vazia"]
            print (json.dumps(lixeiras, indent=4))

            for lixeira in lixeiras:
            print(calculate_volume(lixeira['Forma Geometrica'], lixeira['Dados']))
            

            db.child(condominio).child(sender_name).child('Peso').set(parsed_data[1] - tare)
            db.child(condominio).child(sender_name).child('Volume').set(lixeiras)

    




