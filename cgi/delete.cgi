#!/usr/bin/env python3
import cgi
import os

def sanitize_filename(filename):
    return os.path.basename(filename)

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

form = cgi.FieldStorage()
folder = "orderlog/"
order_time = form.getvalue("orderTime")

if order_time:
    sanitized_filename = sanitize_filename(order_time)
    file_path = os.path.join(folder, sanitized_filename)

    try:
        os.remove(file_path)
        print_html_response("Cancellation Successful", "Your order has been canceled.")
    except FileNotFoundError:
        print_html_response("Cancellation Error", f"Order {file_path} not found. Please check the order time.")
    except PermissionError:
        print_html_response("Cancellation Error", "Permission denied. Cannot cancel the order.")
    except OSError as e:
        print_html_response("Cancellation Error", f"Couldn't cancel your order. Error: {str(e)}")
else:
    print_html_response("Invalid Input", f"{order_time} is not a valid input. Please provide a valid order time.")