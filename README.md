Hướng dẫn sử dụng:
1. Nội dung file test_file.c
Đoạn chương trình trên mô phỏng 1 phần cách mà lệnh ls -l trong linux hoạt động như thế nào cũng như cung cấp 1 số khái niệm về hệ thống file.
Đối với chương trình test_file.c thì nó biểu diễn 5 quá trình của system call
  - Bước 1: mở file bằng lệnh open("text.txt", O_RDWR); chức năng: O_CREAT 0644
  - Bước 2: ghi dữ liệu vào file bằng lệnh write(fd, text, "strlen(text)); chức năng: ghi dữ liệu vào thông qua file descriptor(fd), dữ liệu từ vùng nhớ người dùng sao chép xuống bộ đệm nhân(kernel buffer)
  - Bước 3: Di chuyển con trỏ file về vị trí đầu (offset = 0) bằng lệnh lseek(fd, 0, SEEK_SET); chức năng: giúp việc đọc bắt đầu lại từ đầu file
  - Bước 4: sử dụng lệnh read(fd, buffer, sizeof(buffer) - 1); chức năng: đọc dữ liệu vào bộ nhớ người dùng(user space)
  - Bước 5: sử dụng lệnh close(fd); chức năng: giải phóng tài nguyên, đóng file descriptor

Để có thể hiểu về cách hoạt động của chương trình trên, ta cần quay lại các khái niệm về quản lí file(file management). Tôi sẽ dựa vào hình ảnh 1.1 dưới đây để mô tả 1 số ý tưởng và định nghĩa cần thiết.
<img width="737" height="499" alt="image" src="https://github.com/user-attachments/assets/315f1737-9df7-417c-8ed9-7521ed5ba264" />
 a. Inode table
 Inode table viết tắt của Index node table cho phép ta lưu trữ các dữ liệu metadata của mọi tệp và thư mục trên hệ thống, bao gồm kích thước tệp, quyền sở hữu, thời gian, quyền truy cập, vị trí, ... của dữ liệu trên ổ đĩa 
  Bảng tóm tắt ở hình 1.3 sẽ cho ta thấy cụ thể hơn
  <img width="673" height="220" alt="image" src="https://github.com/user-attachments/assets/8c7f48ff-f829-46c6-8733-a360d5a4c0c4" />
  TỪ hình ảnh 1.2 ta có thể thấy 2 lệnh ls -i và ls -l. Lệnh ls --i cho phép ta in ra số inode của 1 node cụ thể ví dụ như (4326627 test_file.c) thì số inode của nó là 4326627. Còn với lệnh ls -l thì cho thấy được rõ ràng và cụ thể không chỉ về số
  inode mà còn về quyền đọc ghi và thực thi, owner, group, loại file và thời gian modified lần cuối ví dụ như: -rw-rw-r-- 1 vultra_dev vultra_dev  1473 Oct 12 10:57 test_file.c
b. Table of open file (open file table)
  
  Open file table trong Linux là 1 bảng dùng chung cho tất cả các tiến trình, lưu trữ thông tin về tất cả các file đã mở và là 1 bảng gồm 3 thành phần: file offset(vị trí đọc/ghi hiện tại), status flags(trạng thái) và inode pointer(con trỏ inode)
  <img width="631" height="303" alt="image" src="https://github.com/user-attachments/assets/e876d60c-0374-4cb9-9b82-88c67dcbcf67" />
  Mỗi 1 inode pointer sẽ tương ứng với 1 số inode nằm trong bảng mở file, từ đó ứng với các trạng thái và vị trí đọc/ghi của nó. Ví dụ như đối với inode pointer là 142 thì theo bảng ta thấy status_flag của nó là O_RDWR tức là read and write và
  có ví trí hiện tại là 100
c. File descriptor table 
 File descriptor table là 1 mảng chứa các file descriptor, là những số nguyên dương đại diện cho các tệp và các tài nguyên khác mà 1 tiến trình mới mở và nó cũng có 3 thành phần: file descriptor(fd), Fd Flag( giống với Status Flag) và File pointer
 (giống với các chỉ số trong file offset của open file table). Bảng tóm tắt ở hình 1.4 sẽ cho ta thấy cụ thể hơn
 <img width="667" height="276" alt="image" src="https://github.com/user-attachments/assets/1a07433b-174d-4a68-9ca3-30483cea1917" />

 Vậy để có thể tóm tắt quá trình mở file (file opening process) thì ta có thể nói rằng:
  - Mỗi file descriptor là một “tay nắm” mà tiến trình dùng để thao tác file.
  - Tay nắm đó trỏ đến một entry trong bảng file mở (giữ trạng thái mở).
  - Bảng file mở trỏ đến i-node của file (chứa metadata & vị trí dữ liệu thật).
2. Nội dung filestat.c

 



