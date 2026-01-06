# **POLL V SELECT**

## **1. Maximum Number of File Descriptors**

---

## **select()**

>[**Limit**]
>Typically, the maximum number of file descriptors you can monitor is defined by **FD_SETSIZE**, which is usually **1024** on most systems (but this can vary depending on the platform).

>**Implication**
>As the number of file descriptors exceeds this limit, **select()** becomes unusable unless the limit is manually adjusted (which is not recommended for most cases). This can be problematic if your server needs to handle many simultaneous connections.

## **poll()**

>_**No Limit:**_
>There is **no practical limit** on the number of file descriptors you can monitor  
>(apart from system-specific resources).
>
>**poll()** allows you to dynamically allocate an **array of pollfd structures** based on the **number of file descriptors**.

>**Implication**
>As your server grows and handles more connections, **poll()** can **scale** to handle a large number  
>of file descriptors without hitting a predefined limit, unlike **select()**.

<div style="page-break-after: always"></div>

## **2. Efficiency**

---

## **select()**

>**Efficiency**
>As the number of file descriptors grows, **select()** becomes **less efficient** because it needs to **scan the entire bitmask** for **each file descriptor** every time you call it.
>
>This can create **significant performance overhead**, especially when you are monitoring a large number of connections.

>**Implication**
>The more file descriptors you need to monitor, the more CPU time **select()** will consume checking each one, leading to **slower performance** with **high numbers of connections**.

## **poll()**

>**Efficiency**
>**poll()** is **more efficient** because it operates on an array of pollfd structures, each of which has a simple **read/write event flag (events)** and a **result flag (revents)**.
>
>This avoids the need to **scan a bitmask** and makes it **easier** for the system to handle large numbers of file descriptors efficiently.

> **Implication**
>**poll()** is generally more scalable and efficient than **select()** when dealing with a larger number of connections.

<div style="page-break-after: always"></div>

## **3. Flexibility**

---

## **select()**

>**Bitmask**
>In **select()**, you need to set up three separate bitmasks for read, write, and exception events.  
>This can make it harder to manage, especially as you add more file descriptors or want to handle more events.

>**Implication**
>The use of **bitmasks can be cumbersome** when checking for multiple types of events or managing  
>the set of file descriptors in different parts of your code.

## **poll()**

>**Array of pollfd Structures**
>**poll()** works with an array of **pollfd structures**, where each structure represents a  
>**single file descriptor** and specifies the events to monitor (like **POLLIN**, **POLLOUT**, **POLLERR**).  
>
>You can modify the array and events for each file descriptor easily.

>**Implication**
>The use of an array of **pollfd structures** makes **poll()** easier to manage and more flexible for  
>handling multiple types of events and dynamic modifications to the set of file descriptors.

<div style="page-break-after: always"></div>

## **4. Ease of Use**

---

## **select()**

>**Interface Complexity**
>The **select()** API involves managing bitmasks and setting up file descriptor sets (for read, write, and exception checks), which can be **more difficult** and **error-prone** to work with.
>
>You also need to handle the modification of these bitmasks between **select()** calls.

>**Implication**
>It can be **harder to maintain** and understand in more complex systems, especially when working with multiple file descriptors that need to be monitored for different events.

## **poll()**

>**Interface Simplicity**
>The **poll()** function operates on a **simple array of structures**, each containing a file descriptor and the events to monitor.
>
>You pass this array to poll(), which updates it to indicate which file descriptors are ready.

>**Implication**
>**poll()** is **generally simpler** to implement and maintain, making it a **better choice** for projects that require a more straightforward and manageable approach to event handling.

<div style="page-break-after: always"></div>

## 5. Compatibility and Portability

---

## **select()**

>**Widespread Support**
>**select()** is supported on all major Unix-like systems, including Linux, macOS, BSD, etc., and it is available in POSIX.

>**Implication**
>**select()** is **portable**, but its **limitations** in terms of **scalability** and **efficiency** may become a **problem** for large-scale applications.

## **poll()**

>**Widespread Support**
>**poll()** is also supported on Unix-like systems and is **POSIX-compliant**. It’s **more modern** than **select()**,
>and since it's available on both **Linux** and **macOS**, it’s **highly portable** as well.

>**Implication**
>**poll()** is **portable**, and given its **better scalability** and **simpler interface**,
>it's a **more future-proof option** compared to **select()**.

<div style="page-break-after: always"></div>

## **6. Handling Read/Write Events**

---

## **select()**

>**Bitmask for Each Event Type**
>**select()** requires you to **track separate bitmasks** for **read**, **write**, and **error events**.
>This can be a bit more tedious when you need to check multiple types of events at once.

>**Implication**
>Handling both read and write events at the same time is a bit more involved compared to poll(),
>where each event can be tracked directly on the pollfd structure.

## **poll()**

>**Simpler Event Handling**
>**poll()** allows you to check for both read and write events (as well as errors) in a much simpler way by using the events and revents fields in the pollfd structure.
>
>This **simplifies the code significantly** when dealing with multiple event types.

>**Implication**
>Managing read and write events is easier and more intuitive, especially when both
>types of events need to be handled simultaneously.
>
>Given that your project requires handling multiple file descriptors and supports multiple ports, poll()
>is the better choice for scalability, simplicity, and performance.

<div style="page-break-after: always"></div>

## **Why epoll Wouldn't Be Ideal in This Context**

---

## **Multiple File Descriptors Management**

>While **epoll** is more scalable than **poll()** when dealing with large numbers of file descriptors (especially in high-performance servers with thousands of connections), the requirement to use only one **poll()** for all I/O operations precludes the use of epoll here.
>
>**epoll** is designed to allow you to handle multiple events asynchronously, but it can require additional management
>
>(e.g., adding/removing file descriptors, handling edge- vs. level-triggered events) that would complicate your implementation.

## **Linux-Specific**

>**epoll** is a Linux-specific feature, so if you wanted to later port your server to macOS or other platforms, you’d need to adjust >your code to support a different mechanism like kqueue (macOS) or fall back to using **poll()** for compatibility.
>
>Since you are aiming for broad compatibility with your event loop across multiple OSes (Linux and macOS), **poll()** ensures that your server works consistently on both platforms
