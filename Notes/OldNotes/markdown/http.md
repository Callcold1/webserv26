# HTTP/1.1 Request Parsing Flow (Detailed)

This document outlines the structured process for parsing HTTP/1.1 requests according to **RFC 9110, RFC 9111, and RFC 9112**.

---

## 🚀 HTTP/1.1 Request Parsing Flow

### **1️⃣ Receive Request**

- Read incoming bytes from the socket until **CRLF (`\r\n`)** is detected at the end of the request line.
- Ensure the request does not exceed **server limits** (e.g., header size, total request size).

---

### **2️⃣ Parse Request Line**

The request line format **must** be:

``` METHOD SP REQUEST-TARGET SP HTTP-VERSION CRLF ```

**Example:**

``` GET /index.html HTTP/1.1\r\n ```

#### 🔹 **Validation Checks:**

✅ **Method (first word in line):**

- Must be a **known and valid HTTP/1.1 method** (case-sensitive).
- Common valid methods: `GET`, `POST`, `DELETE`.
- **Invalid method?** → Respond `501 Not Implemented`.

✅ **Request Target (second part):**

- Defines the resource being requested. **Valid formats include:**
     1. **Absolute Path** (`/resource/path` or `/index.html`)
     2. **Absolute URI** (`http://example.com/index.html`) (used in proxies)
     3. **Asterisk (`*`)** (Only for `OPTIONS` method)
- **Invalid format?** → Respond `400 Bad Request`.

✅ **HTTP Version (last part):**

- Must be exactly `HTTP/1.1`.
- **Invalid version?** → Respond `505 HTTP Version Not Supported`.

🚨 **If any of these fail, reject with `400 Bad Request`.**

---

### **3️⃣ Parse Headers**

Headers **must** follow this format:

```Field-Name: Field-Value CRLF```

#### 🔹 **Validation Checks:**

✅ **Field-Name:**

- Case-insensitive (`Content-Type` = `content-type`).
- No spaces or special characters, only letters, numbers, and `-`.
- **Invalid name?** → Respond `400 Bad Request`.

✅ **Field-Value:**

- No control characters except spaces, `\t`, or quoted values.
- **Malformed values?** → Respond `400 Bad Request`.

✅ **Header Line Termination:**

- Each header **must** end with `CRLF`.
- Empty line (`CRLF`) marks the **end of headers**.

🚨 **Malformed headers → `400 Bad Request`.**

---

### **4️⃣ Determine Message Body Presence**

#### 🔹 **Based on Method:**

- **GET** → 🚫 **No body allowed** (ignore if present).
- **POST/DELETE** → A body **may be present**.

#### 🔹 **How to Determine Body Presence:**

1. `Content-Length: X` → Exact number of bytes expected.
2. `Transfer-Encoding: chunked` → Body is chunked (must be processed accordingly).
3. **Neither header present?** → Assume **no body**.

🚨 **Invalid Content-Length?** → Respond `411 Length Required`.

---

### **5️⃣ Parse Message Body (If Present)**

#### 🔹 **Based on Encoding:**

✅ **Content-Length Present?**

- Read exactly **X bytes** from the socket.
- **Less or more bytes received?** → `400 Bad Request`.

✅ **Transfer-Encoding: chunked?**

- Expect **chunked transfer encoding** format:

    ```Chunk-Size CRLF```
    ```Chunk-Data CRLF```

- Ends when a **zero-length chunk** (`0\r\n`) is received.
- **Malformed chunks?** → `400 Bad Request`.

🚨 **If malformed, reject with `400 Bad Request`.**

---

### **6️⃣ Pass Parsed Request to Application Logic**

- Match request **method and target** with application logic.
- Route to the appropriate handler.
- Execute the requested action.

---

### **7️⃣ Generate and Send Response**

- Prepare an **HTTP response** based on the request.
- Ensure the response follows:

```HTTP/1.1 STATUS CODE CRLF```
```Headers CRLF```
```[Optional Body]```

- Send data to the client.

---

### **📌 Summary of Error Handling:**

| **Error Type** | **Response Code** | **Cause** |
|--------------|----------------|---------|
| Invalid HTTP Method | `501 Not Implemented` | Unrecognized method name. |
| Invalid Request Target | `400 Bad Request` | Malformed or missing URI. |
| Invalid HTTP Version | `505 HTTP Version Not Supported` | Must be `HTTP/1.1`. |
| Malformed Headers | `400 Bad Request` | Incorrect format or illegal characters. |
| Missing Content-Length | `411 Length Required` | POST/DELETE needs a valid `Content-Length`. |
| Chunked Encoding Error | `400 Bad Request` | Malformed chunked body. |

---

### **🛠 Final Thoughts**

This detailed process ensures that an **HTTP/1.1 server correctly parses incoming requests**, **validates all required components**, and **handles errors properly** before forwarding the request for processing.

Would you like any specific areas expanded further? 😊
