from cryptography.fernet import Fernet
key = "V9TcZqQXfTm53ri9vPoybLjQN0m0jtrFh09QXRZCi_s="
f = Fernet(key)

# takes Python plaintext string and returns base-64 Fernet encrypted string
def encrypt(message):
    message_bytes = message.encode()
    return f.encrypt(message_bytes)

# takes base-64 Fernet encrypted string and returns plaintext
def decrypt(encrypted):
    decrypted = f.decrypt(encrypted)
    return decrypted.decode()

encrypted = encrypt("my message")
print(encrypted)
print(decrypt(encrypted))

#print(key)
#print(f)
#print(token)
#print(f.decrypt(token))
