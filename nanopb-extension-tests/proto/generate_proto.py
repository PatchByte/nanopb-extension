import os, shutil

protoc_path = "..\\..\\Vendors\\nanopb\\generator\\protoc"

if not os.name == "nt":
    protoc_path = "./../../Vendors/nanopb/generator/protoc"

proto_targets = [
    "test.proto"
]

def generate_with_protoc(file_directory, file_proto):
    command = protoc_path + " --proto_path="+file_directory+" --nanopb_out="+file_directory+" "+file_proto
    os.system(command)

for target in proto_targets:
    generate_with_protoc(".", target)