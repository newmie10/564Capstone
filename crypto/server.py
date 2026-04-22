from cryptography.fernet import Fernet
key = Fernet.generate_key()
f = Fernet(key)
token = f.encrypt(b"my deep dark secret")
print(key)
print(f)
print(token)
print(f.decrypt(token))
