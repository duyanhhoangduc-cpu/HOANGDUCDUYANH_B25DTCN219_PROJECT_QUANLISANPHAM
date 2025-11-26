#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#define MAX 100
#define PRODUCT_ID_LEN 10
#define PRODUCT_NAME_LEN 50
#define PRODUCT_UNIT_LEN 16
#define MAX_CHOICE_LEN 10 // Kich thuoc de nhap lua chon menu

// Khai bao cau truc Product (Hang hoa)
struct Product {
    char productId[PRODUCT_ID_LEN]; // Ma hang hoa (Duy nhat. Khoa chinh)
    char name[PRODUCT_NAME_LEN];    // Ten hang hoa
    char unit[PRODUCT_UNIT_LEN];    // Don vi hang hoa (Cai/Hop...)
    int qty;                        // So luong ton kho
    int status;                     // Trang thai (1: Active, 0: Locked/Xoa mem)
};

// Khai bao cau truc Transaction (Xuat/Nhap hang hoa)
struct Transaction {
    char transId[20];
    char productId[PRODUCT_ID_LEN];
    char type[5]; // IN = nhap / OUT = xuat
    char date[15]; // Thoi gian giao dich.
};

struct Product products[MAX];
int productCount = 0;
void display_menu();
int isUniqueId(const char* id);
void add_new_product();
void display_product_list();
struct Product* find_product_by_id(const char* id);
void update_product_info();
void validate_and_update(struct Product* p);
void lock_product_status();
void search_product();

// Ham tien ich de nhap chuoi an toan va loai bo ky tu xuong dong.
void safe_input(char* buffer, int size) {
    fgets(buffer, size, stdin); 
    buffer[strcspn(buffer, "\n")] = 0; 
}

// Ham xoa bo dem dau vao (cho scanf)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// CHUC NANG CHINHHH!!!!!!!!
int main() {
    int choice;
    char choice_input[MAX_CHOICE_LEN];
    do {
        display_menu();
        printf("Nhap lua chon cua ban: ");
        

        safe_input(choice_input, MAX_CHOICE_LEN);

//2Kiem tra neu la chuoi rong (chi bam Enter)
        if (strlen(choice_input) == 0) {
            printf("\nKO DUOC TREU DUA !! NHAP LAI NHUNG THU CAN NHAP.\n");
            choice = -1; // Ðat choice la giá tri không hop le
            continue;
        }
        
//Su dung sscanf de chuyen doi chuoi sang so nguyen
        if (sscanf(choice_input, "%d", &choice) != 1) {
            printf("\nVUI LONG CHI NHAP 1 SO ^^.\n");
            choice = -1; // Ð?t choice là giá tr không hli
            continue;
        }

// Khong can clear_input_buffer() hay while(getchar() != '\n') nua
// vì safe_input dã xu lys toàn bo input buffer.
        
        switch (choice) {
            case 1:
                add_new_product();
                break;
            case 2:
                display_product_list();
                break;
            case 3:
                update_product_info();
                break;
            case 4:
                lock_product_status();
                break;
            case 5:
                search_product();
                break;
            case 6:
                
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:

                break;
            case 0:
                printf("\n--- Thoat chuong trinh. Tam biet KO bao gio gap lai hehe^^! ---\n");
                break;
            default:
                printf("\nLua chon khong hop le. Vui long nhap lai.\n");
        }
        printf("\n");
    } while (choice != 0);

    return 0;
}

void display_menu() {
    printf("\n-------------HE THONG QUAN LY HANG HOA--------------\n");
    printf("1. Them hang hoa moi\n");
    printf("2. Hien thi danh sach hang hoa\n");
    printf("3. Cap nhat thong tin hang hoa (Sua Name, Unit, Qty)\n");
    printf("4. Quan li trang thai (khoa/xoa)\n");
    printf("5. tra cuu (Tim Kiem)\n");
    printf("6. danh sach (Phan Trang)\n");
    printf("7. sap xep danh sach\n");
    printf("8. giao dich xuat/nhap hang hoa\n");
    printf("9. lich su xuat/nhap\n");
    printf("0. Thoat\n");
    printf("-----------------------------------------------------\n");
}

int isUniqueId(const char* id) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].productId, id) == 0) {
            return 0; // Da tim thay ID trung lap
        }
    }
    return 1; // ID duy nhat
}

//Them hang hoa moi vao danh sach.
//Bao gom kiem tra ID duy nhat va validate cac truong bat buoc.
void add_new_product() {
    if (productCount >= MAX) {
        printf("Danh sach san pham da day!\n");
        return;
    }

    struct Product newProduct;
    char tempId[PRODUCT_ID_LEN];

    printf("\n--- THEM HANG HOA MOI ---\n");

    // 1. Nhap va Validate productId (Duy nhat va Khong rong)
    do {
        printf("Nhap ID san pham (toi da %d ky tu): ", PRODUCT_ID_LEN - 1);
        safe_input(tempId, PRODUCT_ID_LEN); 

        if (strlen(tempId) == 0) {
            printf("ID khong duocc de trong! Vui longa nhap lai.\n");
        } else if (!isUniqueId(tempId)) {
            printf("ID '%s' da ton tai. Vui long nhap ID khac!\n", tempId);
        } 
    } while (strlen(tempId) == 0 || !isUniqueId(tempId));
    strcpy(newProduct.productId, tempId);

    // 2. Nhap va Validate Name (Khong rong)
    do {
        printf("Nhap Ten hang hoa (khong duoc de trong): ");
        safe_input(newProduct.name, PRODUCT_NAME_LEN);
        
        if (strlen(newProduct.name) == 0) {
            printf("[LOI ROI EM] Ten hang hoa khog duoc de trong! Vui long nhap lai.\n");
        }
    } while (strlen(newProduct.name) == 0);

    // 3. Nhap va Validate Unit (Khong rong)
    do {
        printf("Nhap Don vi hang hoa : ");
        safe_input(newProduct.unit, PRODUCT_UNIT_LEN);

        if (strlen(newProduct.unit) == 0) {
            printf("[LOI ROI EM] Don vi hang hoa khong duoc de trong! Vui long nhap lai.\n");
        }
    } while (strlen(newProduct.unit) == 0);

    // 4. Nhap va Validate Qty (>=0)
    do {
        printf("Nhap So luong ton kho: ");
        if (scanf("%d", &newProduct.qty) != 1) {
             printf("[ERRORRRR] So luong phai la so nguyen va >= 0!\n");
             clear_input_buffer(); 
             newProduct.qty = -1; 
             continue;
        }
        clear_input_buffer(); 

        if (newProduct.qty < 0) {
            printf("[ERRORRR] So luong ton kho phai lon hon hoac bang 0! Vui long nhap lai.\n");
        }
    } while (newProduct.qty < 0);

    // Mac dinh Status la 1 (Con su dung)
    newProduct.status = 1;

    // Luu san pham vao mang
    products[productCount] = newProduct;
    productCount++;

    printf("\nThem hang hoa '%s' thanh cong! So luong hien tai: %d\n", newProduct.name, productCount);
}

// Hien thi danh sach tat ca hang hoa hien co
void display_product_list() {
    if (productCount == 0) {
        printf("\n Danh sach hang hoa trong\n");
        return;
    }

    printf("\n--- DANH SACH HANG HOA (Tong: %d) ---\n", productCount);
    printf("%-5s| %-10s| %-40s| %-10s| %-5s| %-6s\n", 
           "STT", "ID", "TEN HANG HOA", "DON VI", "QTY", "STATUS");
    printf("----------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < productCount; i++) {
        printf("%-5d| %-10s| %-40s| %-10s| %-5d| %-6s\n", 
               i + 1,
               products[i].productId, 
               products[i].name, 
               products[i].unit, 
               products[i].qty, 
               products[i].status == 1 ? "Active" : "Locked");
    }
    printf("----------------------------------------------------------------------------------\n");
}

// Tim kiem hang hoa theo ID
struct Product* find_product_by_id(const char* id) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].productId, id) == 0) {
            return &products[i]; // Tra ve dia chi cua Product tim duoc
        }
    }
    return NULL; // Khong tim thay
}

// Chuc nang cap nhat thong tin hang hoa
void update_product_info() {
    char searchId[PRODUCT_ID_LEN];
    struct Product* productToUpdate = NULL;

    printf("\n--- CAP NHAT THONG TIN HANG HOA ---\n");
    printf("Nhap ID san pham can cap nhat: ");
    safe_input(searchId, PRODUCT_ID_LEN);

    // Kiem tra ID khong duoc rong khi tim kiem
    if (strlen(searchId) == 0) {
        printf("[LOI ROI] ID khong duoc de trong. Khong the tim kiem.\n");
        return;
    }

    productToUpdate = find_product_by_id(searchId);

    if (productToUpdate == NULL) {
        printf("ERRORRRRR^^ Khong tim thay hang hoa voi ID '%s'.\n", searchId);
        return;
    }

    // Khong cho phep cap nhat neu san pham dang bi khoa/xoa mem
    if (productToUpdate->status == 0) {
        printf("[INFO] Hang hoa ID '%s' da bi KHOA/XOA MEM. Khong the cap nhat!\n", searchId);
        return;
    }
    
    printf("\n--- THONG TIN HANG HOA HIEN TAI ---\n");
    printf("ID: %s\n", productToUpdate->productId);
    printf("Ten hien tai: %s\n", productToUpdate->name);
    printf("Don vi hien tai: %s\n", productToUpdate->unit);
    printf("So luong ton kho hien tai: %d\n", productToUpdate->qty);
    printf("-----------------------------------\n");

    // Thuc hien viec nhap va validate du lieu khi cap nhat.
    validate_and_update(productToUpdate);
}

// Thuc hien viec nhap va validate du lieu khi cap nhat
void validate_and_update(struct Product* p) {
    char tempStr[PRODUCT_NAME_LEN];
    int tempQty;
    int isUpdated = 0;

    printf("\nBo qua cap nhat bang cach nhap '-' (dau gach ngang)\n");

    // 1. Cap nhat Ten hang hoa (Khong rong)
    do {
        printf("Nhap Ten hang hoa moi (hoac '-' de bo qua): ");
        safe_input(tempStr, PRODUCT_NAME_LEN);

        if (strcmp(tempStr, "-") == 0) {
            printf("[INFO] Bo qua cap nhat Ten hang hoa.\n");
            break;
        }

        // Kiem tra neu la chuoi rong (chi bam Enter)
        if (strlen(tempStr) == 0) {
            printf("Ten hang hoa khong duoc de trong! Vui long nhap lai.\n");
        } else {
            strcpy(p->name, tempStr);
            isUpdated = 1;
            break;
        }
    } while (1);

    // 2. Cap nhat Don vi hang hoa (Khong rong)
    do {
        printf("Nhap Don vi hang hoa moi (hoac '-' de bo qua): ");
        safe_input(tempStr, PRODUCT_UNIT_LEN);

        if (strcmp(tempStr, "-") == 0) {
            printf(" LOA:Bo qua cap nhat Don vi hang hoa.\n");
            break;
        }

        // Kiem tra neu la chuoi rong (chi bam Enter)
        if (strlen(tempStr) == 0) {
            printf("[loiloi] Don vi hang hoa khong duoc de trong! Vui long nhap lai.\n");
        } else {
            strcpy(p->unit, tempStr);
            isUpdated = 1;
            break;
        }
    } while (1);

    // 3. Cap nhat So luong ton kho (>=0)
    do {
        printf("Nhap So luong ton kho moi (hoac '-' de bo qua): ");
        
        char qty_input[20];
        if (scanf("%s", qty_input) != 1) {
             clear_input_buffer(); 
             continue;
        }
        clear_input_buffer();

        if (strcmp(qty_input, "-") == 0) {
            printf("Bo qua cap nhat So luong ton kho.\n");
            break;
        }
        
        // Thu chuyen doi sang so nguyen
        if (sscanf(qty_input, "%d", &tempQty) == 1) {
            if (tempQty < 0) {
                printf("[LOI R?I] So luong ton kho phai lon hon hoac bang 0! Vui long nhap lai.\n");
            } else {
                p->qty = tempQty;
                isUpdated = 1;
                break;
            }
        } else {
            printf("[SIUUUUU] So luong phai la so nguyen va >= 0! Vui long nhap lai.\n");
        }
    } while (1);
    
    if (isUpdated) {
        printf("\n Cap nhat thong tin hang hoa '%s' (ID: %s) thanh cong!\n", p->name, p->productId);
    } else {
        printf("\n Khong co thong tin nao duoc cap nhat.\n");
    }
}

void lock_product_status() {
    char searchId[PRODUCT_ID_LEN];
    struct Product* p = NULL;

    printf("\n--- KHOA/MO KHOA TRANG THAI HANG HOA ---\n");
    printf("Nhap ID hang hoa can thay doi trang thai (Lock/Unlock): ");
    safe_input(searchId, PRODUCT_ID_LEN);

    if (strlen(searchId) == 0) {
        printf("[LOI] ID khong duoc de trong.\n");
        return;
    }

    p = find_product_by_id(searchId);

    if (p == NULL) {
        printf("Khong tim thay hang hoa voi ID '%s'.\n", searchId);
        return;
    }

    // Doi trang thai hien tai
    if (p->status == 1) {
        p->status = 0; // Khoa (Soft Delete)
        printf("\n Hang hoa ID '%s' (%s) da bi KHOA/XOA MEM.\n", p->productId, p->name);
        printf("=> Hang hoa nay hien khong the thuc hien giao dich.\n");
    } else {
        p->status = 1; // Mo khoa
        printf("\nHang hoa ID '%s' (%s) da duoc MO KHOA/KICH HOAT lai.\n", p->productId, p->name);
    }
}

// CHUC NANG 5: TRA CUU (TIM KIEM)
void search_product() {
    char search_term[PRODUCT_NAME_LEN];
    int found_count = 0;

    printf("\n--- TRA CUU HANG HOA THEO ID HOAC TEN ---\n");
    printf("Nhap ID (chinh xac) hoac Ten hang hoa (gan dung): ");
    safe_input(search_term, PRODUCT_NAME_LEN);

    if (strlen(search_term) == 0) {
        printf("Vui long nhap ID hoac Ten de tim kiem.\n");
        return;
    }
    // Tao chuoi tim kiem in hoa de tim kiem gan dung (insensitive search)
    char upper_search_term[PRODUCT_NAME_LEN];
    strcpy(upper_search_term, search_term);
    for (int i = 0; upper_search_term[i]; i++) {
        if (upper_search_term[i] >= 'a' && upper_search_term[i] <= 'z') {
            upper_search_term[i] = upper_search_term[i] - 32; // Chuyen thanh in hoa
        }
    }

    printf("\n--- KET QUA TIM KIEM ---\n");
    printf("%-5s| %-10s| %-40s| %-10s| %-5s| %-6s\n", 
           "STT", "ID", "TEN HANG HOA", "DON VI", "QTY", "STATUS");
    printf("----------------------------------------------------------------------------------\n");

    for (int i = 0; i < productCount; i++) {
        // 1. Tim theo ID chinh xac
        if (strcmp(products[i].productId, search_term) == 0) {
            found_count++;
             printf("%-5d| %-10s| %-40s| %-10s| %-5d| %-6s\n", 
                   found_count,
                   products[i].productId, 
                   products[i].name, 
                   products[i].unit, 
                   products[i].qty, 
                   products[i].status == 1 ? "Active" : "Locked");
            continue; // Da tim thay theo ID, chuyen sang san pham tiep theo
        }

        // 2. Tim theo Ten gan dung (su dung strstr de kiem tra chuoi con)
        char upper_product_name[PRODUCT_NAME_LEN];
        strcpy(upper_product_name, products[i].name);
        for (int j = 0; upper_product_name[j]; j++) {
            if (upper_product_name[j] >= 'a' && upper_product_name[j] <= 'z') {
                upper_product_name[j] = upper_product_name[j] - 32; // Chuyen thanh in hoa
            }
        }
        
        // Kiem tra xem chuoi tim kiem co phai la chuoi con trong ten san pham khong
        if (strstr(upper_product_name, upper_search_term) != NULL) {
            found_count++;
            printf("%-5d| %-10s| %-40s| %-10s| %-5d| %-6s\n", 
                   found_count,
                   products[i].productId, 
                   products[i].name, 
                   products[i].unit, 
                   products[i].qty, 
                   products[i].status == 1 ? "Active" : "Locked");
        }
    }

    printf("----------------------------------------------------------------------------------\n");
    if (found_count == 0) {
        printf("Khong tim thay ket qua nao cho '%s'.\n", search_term);
    } else {
        printf("Tim thay tong cong %d ket qua.\n", found_count);
    }
}
