import json

def read_settings():

    with open("settings.json", "r") as settings :
        settings_data = json.loads(settings.read())
        return settings_data

def parse_payload(payload):
    raw_data = bytes(payload.message).decode("utf-8")

    parsed_data = [str(data) for data in raw_data.split("$")]

    return parsed_data

def on_recv(payload):

    sender_id = payload.header_from

    settings = read_settings()
    trash_cans = settings['information']['Lixeiras']
    condominio = settings['information']['Nome do Condomínio']

    for can in trash_can:
        if can["ID"] == sender_id:
            sender_name = can["Nome"]

    parsed_data = parse_payload(payload)

    db.child(condiminio).child(sender_name).child("Peso").set(parsed_data[1])
    db.child(condiminio).child(sender_name).child("Altura").set(parsed_data[2])