#!/usr/bin/env python3

import cgi
import os

# Set the directory where uploaded files will be stored
upload_dir = os.environ.get("UPLOAD_DIR")

# Create a FieldStorage instance
form = cgi.FieldStorage()

def print_html_response(title, message):
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print(f"<title>{title}</title>")
    print("</head>")
    print("<body>")
    print(f"<h3>{title}</h3>")
    print(f"<p>{message}</p>")
    print("</body>")
    print("</html>")

# Check if the file field is present
if "fileToUpload" not in form:
    print_html_response("Nope", "No file was uploaded")
else:
    # Retrieve the uploaded file
    file_item = form["fileToUpload"]

    # Check if the file was uploaded
    if file_item.filename:
        # Securely create a full file path
        file_path = os.path.join(upload_dir, os.path.basename(file_item.filename))

        # Open the file in write-binary mode
        with open(file_path, "wb") as f:
            # Write the file content
            f.write(file_item.file.read())

        print_html_response("Success", f"File {file_item.filename} uploaded successfully</h1>")
    else:
        print_html_response("Sorry", "File was not uploaded")
