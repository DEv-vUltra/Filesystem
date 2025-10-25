<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="UTF-8">
  <title>README - test_file.c & filestat.c</title>
</head>
<body style="font-family: Arial, sans-serif; line-height: 1.6; margin: 20px;">
  <h1>Hướng dẫn sử dụng</h1>

  <h2>1. Nội dung file <code>test_file.c</code></h2>
  <p>
    Đoạn chương trình trên mô phỏng 1 phần cách mà lệnh <code>ls -l</code> trong linux hoạt động như thế nào cũng như cung cấp 1 số khái niệm về hệ thống file.
    Đối với chương trình <code>test_file.c</code> thì nó biểu diễn 5 quá trình của system call:
  </p>
  <ul>
    <li><b>Bước 1:</b> mở file bằng lệnh <code>open("text.txt", O_RDWR);</code> chức năng: O_CREAT 0644</li>
    <li><b>Bước 2:</b> ghi dữ liệu vào file bằng lệnh <code>write(fd, text, strlen(text));</code> chức năng: ghi dữ liệu vào thông qua file descriptor(fd), dữ liệu từ vùng nhớ người dùng sao chép xuống bộ đệm nhân(kernel buffer)</li>
    <li><b>Bước 3:</b> Di chuyển con trỏ file về vị trí đầu (offset = 0) bằng lệnh <code>lseek(fd, 0, SEEK_SET);</code> chức năng: giúp việc đọc bắt đầu lại từ đầu file</li>
    <li><b>Bước 4:</b> sử dụng lệnh <code>read(fd, buffer, sizeof(buffer) - 1);</code> chức năng: đọc dữ liệu vào bộ nhớ người dùng(user space)</li>
    <li><b>Bước 5:</b> sử dụng lệnh <code>close(fd);</code> chức năng: giải phóng tài nguyên, đóng file descriptor</li>
  </ul>

  <p>
    Để có thể hiểu về cách hoạt động của chương trình trên, ta cần quay lại các khái niệm về quản lí file (file management). Tôi sẽ dựa vào hình ảnh 1.1 dưới đây để mô tả 1 số ý tưởng và định nghĩa cần thiết.
  </p>

  <img width="737" height="499" alt="image" src="https://github.com/user-attachments/assets/315f1737-9df7-417c-8ed9-7521ed5ba264" />

  <h3>a. Inode table</h3>
  <p>
    Inode table viết tắt của Index node table cho phép ta lưu trữ các dữ liệu metadata của mọi tệp và thư mục trên hệ thống, bao gồm kích thước tệp, quyền sở hữu, thời gian, quyền truy cập, vị trí, ... của dữ liệu trên ổ đĩa.
  </p>
  <p>Bảng tóm tắt ở hình 1.3 sẽ cho ta thấy cụ thể hơn:</p>
  <img width="673" height="220" alt="image" src="https://github.com/user-attachments/assets/8c7f48ff-f829-46c6-8733-a360d5a4c0c4" />

  <p>
    Từ hình ảnh 1.2 ta có thể thấy 2 lệnh <code>ls -i</code> và <code>ls -l</code>.
    Lệnh <code>ls -i</code> cho phép ta in ra số inode của 1 node cụ thể ví dụ như (4326627 test_file.c) thì số inode của nó là 4326627.
    Còn với lệnh <code>ls -l</code> thì cho thấy được rõ ràng và cụ thể không chỉ về số inode mà còn về quyền đọc ghi và thực thi, owner, group, loại file và thời gian modified lần cuối ví dụ như:
    <br>
    <code>-rw-rw-r-- 1 vultra_dev vultra_dev 1473 Oct 12 10:57 test_file.c</code>
  </p>

  <h3>b. Table of open file (open file table)</h3>
  <p>
    Open file table trong Linux là 1 bảng dùng chung cho tất cả các tiến trình, lưu trữ thông tin về tất cả các file đã mở và là 1 bảng gồm 3 thành phần: file offset (vị trí đọc/ghi hiện tại), status flags (trạng thái) và inode pointer (con trỏ inode).
  </p>
  <img width="631" height="303" alt="image" src="https://github.com/user-attachments/assets/e876d60c-0374-4cb9-9b82-88c67dcbcf67" />
  <p>
    Mỗi 1 inode pointer sẽ tương ứng với 1 số inode nằm trong bảng mở file, từ đó ứng với các trạng thái và vị trí đọc/ghi của nó. Ví dụ như đối với inode pointer là 142 thì theo bảng ta thấy status_flag của nó là O_RDWR tức là read and write và có ví trí hiện tại là 100.
  </p>

  <h3>c. File descriptor table</h3>
  <p>
    File descriptor table là 1 mảng chứa các file descriptor, là những số nguyên dương đại diện cho các tệp và các tài nguyên khác mà 1 tiến trình mới mở và nó cũng có 3 thành phần: file descriptor(fd), Fd Flag (giống với Status Flag) và File pointer (giống với các chỉ số trong file offset của open file table).
  </p>
  <img width="667" height="276" alt="image" src="https://github.com/user-attachments/assets/1a07433b-174d-4a68-9ca3-30483cea1917" />

  <p>
    Vậy để có thể tóm tắt quá trình mở file (file opening process) thì ta có thể nói rằng:
  </p>
  <ul>
    <li>Mỗi file descriptor là một “tay nắm” mà tiến trình dùng để thao tác file.</li>
    <li>Tay nắm đó trỏ đến một entry trong bảng file mở (giữ trạng thái mở).</li>
    <li>Bảng file mở trỏ đến i-node của file (chứa metadata & vị trí dữ liệu thật).</li>
  </ul>

  <h2>2. Nội dung <code>filestat.c</code></h2>
  <p><b>Các yêu cầu của chương trình:</b></p>
  <ul>
    <li>Chương trình phải nhận chính xác 1 tham số từ dòng lệnh, đó là đường dẫn đến 1 tệp hay 1 thư mục và nếu không có tham số nào được cung cấp thì sẽ in ra hướng dẫn sử dụng.</li>
    <li>Các nội dung mà chương trình sẽ hiển thị:</li>
    <ul>
      <li><b>File path:</b> Đường dẫn mà người dùng đã nhập</li>
      <li><b>File type:</b> Loại của đối tượng tức là các loại như regular file, directory, symbolic link, vv... (sử dụng các macro từ <code>&lt;sys/stat.h&gt;</code>)</li>
      <li><b>Size:</b> Kích thước của tệp, lấy từ trường <code>st_size</code></li>
      <li><b>Last modified:</b> Thời gian sửa đổi tệp lần cuối, lấy giá trị <code>time_t</code> từ trường <code>st_mtime</code>, chuyển đổi timestamp này thành một chuỗi ngày-tháng-năm, giờ-phút-giây mà con người có thể đọc được</li>
    </ul>
  </ul>
</body>
</html>
